
#include "App.h"
#include "AppException.h"
#include "Bindables.h"

struct Vertex
{
	float x;
	float y;
	float z;
	float u;
	float v;
};

struct VS_cb
{
	DirectX::XMMATRIX transform;
};

struct PS_cb
{
	struct { float r, g, b, a; } colors[6];
};

static std::vector<std::unique_ptr<Bindable>> bindables;
std::unique_ptr<IndexBuffer> ibo;
std::unique_ptr<VertexConstantBuffer<VS_cb>> vcbo;

App::App()
	: m_Window({ 1280, 720, L"DirectX11" })
{
	using namespace DirectX;
	
	std::vector<Vertex> vertices = {
		{  1.0f,  1.0f, -1.0f,		1.0f, 0.0f }, // 0 front
		{  1.0f,  1.0f, -1.0f,		0.0f, 0.0f },
		{  1.0f,  1.0f, -1.0f,		1.0f, 1.0f },
		
		{  1.0f,  1.0f,  1.0f,		1.0f, 0.0f }, // 1 right
		{  1.0f,  1.0f,  1.0f,		0.0f, 0.0f },
		{  1.0f,  1.0f,  1.0f,		1.0f, 0.0f },
		
		{  1.0f, -1.0f, -1.0f,		1.0f, 1.0f }, // 2 front
		{  1.0f, -1.0f, -1.0f,		0.0f, 1.0f },
		{  1.0f, -1.0f, -1.0f,		1.0f, 0.0f },
		
		{  1.0f, -1.0f,  1.0f,		1.0f, 1.0f }, // 3 right
		{  1.0f, -1.0f,  1.0f,		0.0f, 1.0f },
		{  1.0f, -1.0f,  1.0f,		1.0f, 1.0f },
		
		{ -1.0f,  1.0f, -1.0f,		0.0f, 0.0f }, // 4 front
		{ -1.0f,  1.0f, -1.0f,		1.0f, 0.0f },
		{ -1.0f,  1.0f, -1.0f,		0.0f, 1.0f },
		
		{ -1.0f,  1.0f,  1.0f,		1.0f, 0.0f }, // 5 back
		{ -1.0f,  1.0f,  1.0f,		0.0f, 0.0f },
		{ -1.0f,  1.0f,  1.0f,		0.0f, 0.0f },
		
		{ -1.0f, -1.0f, -1.0f,		0.0f, 1.0f }, // 6 front
		{ -1.0f, -1.0f, -1.0f,		1.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f,		0.0f, 0.0f },
		
		{ -1.0f, -1.0f,  1.0f,		1.0f, 1.0f }, // 7 back
		{ -1.0f, -1.0f,  1.0f,		0.0f, 1.0f },
		{ -1.0f, -1.0f,  1.0f,		0.0f, 1.0f }
	};

	std::vector<unsigned short> indices = {
		6*3+0, 4*3+0, 0*3+0, 0*3+0, 2*3+0, 6*3+0, // front
		2*3+1, 0*3+1, 1*3+0, 1*3+0, 3*3+0, 2*3+1, // right
		3*3+1, 1*3+1, 5*3+0, 5*3+0, 7*3+0, 3*3+1, // back
		7*3+1, 5*3+1, 4*3+1, 4*3+1, 6*3+1, 7*3+1, // left
		4*3+2, 5*3+2, 1*3+2, 1*3+2, 0*3+2, 4*3+2, // top
		7*3+2, 6*3+2, 2*3+2, 2*3+2, 3*3+2, 7*3+2  // bottom
	};

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoords", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	VS_cb vscb = {};
	vscb.transform = XMMatrixTranspose(
						XMMatrixTranslation(0.f, 0.f, 4.f) *
						XMMatrixPerspectiveLH(1.f, 9.f / 16.f, 0.5f, 10.f));

	PS_cb pscb;
	pscb.colors[0] = {1.0f,0.0f,1.0f,0.0f};
	pscb.colors[1] = {0.0f,1.0f,1.0f,0.0f};
	pscb.colors[2] = {0.0f,0.0f,1.0f,0.0f};
	pscb.colors[3] = {1.0f,0.0f,0.0f,0.0f};
	pscb.colors[4] = {0.0f,1.0f,0.0f,0.0f};
	pscb.colors[5] = {1.0f,1.0f,0.0f,0.0f};

	bindables.push_back(std::make_unique<VertexBuffer>(m_Window.GetGfx(), vertices));
	ibo = std::make_unique<IndexBuffer>(m_Window.GetGfx(), indices);
	bindables.push_back(std::make_unique<PixelShader>(m_Window.GetGfx(), L"res/shaders/PixelShader.cso"));

	auto pvs = std::make_unique<VertexShader>(m_Window.GetGfx(), L"res/shaders/VertexShader.cso");
	bindables.push_back(std::make_unique<InputLayout>(m_Window.GetGfx(), ied, pvs->GetBlob()));
	bindables.push_back(std::move(pvs));

	bindables.push_back(std::make_unique<Topology>(m_Window.GetGfx(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	vcbo = std::make_unique<VertexConstantBuffer<VS_cb>>(m_Window.GetGfx(), vscb);
	bindables.push_back(std::make_unique<Texture>(m_Window.GetGfx(), "res/textures/container.png"));
	bindables.push_back(std::make_unique<Sampler>(m_Window.GetGfx()));
	// bindables.push_back(std::make_unique<PixelConstantBuffer<PS_cb>>(m_Window.GetGfx(), pscb));

}

int App::Run()
{
	try
	{
		std::optional<int> result;
		m_Window.ShowWindow();

		while (!result.has_value())
		{
			result = m_Window.PollEvents();

			App::Update();
		}

		return result.value();
	}
	catch (const MyAppException&)
	{
		__debugbreak();
		return 0;
	}
	catch (const std::exception&)
	{
		// MessageBox(nullptr, e.what(), L"Standard Exception", MB_OK | MB_ICONERROR);
		__debugbreak();
		return 0;
	}
	catch (...)
	{
		// MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONERROR);
		__debugbreak();
		return 0;
	}
}

void App::Update()
{

	const float angle = m_Timer.Peek() / 1.25f * 3.14159f;

	m_Window.GetGfx().ClearBuffer(0.2f, 0.2f, 0.2f);

	for (auto &bindable : bindables)
		bindable->Bind(m_Window.GetGfx());
	ibo->Bind(m_Window.GetGfx());
	vcbo->Bind(m_Window.GetGfx());

	srand(0);
	for (int i = 0; i < 1000; ++i)
	{
		using namespace DirectX;
		
		float scale = ((rand() % 50) + 50) / 1000.f;
		float x = ((rand() % 200) - 100) / 33.f;
		float y = ((rand() % 200) - 100) / 33.f;
		float z = ((rand() % 90) + 10) / 10.f;
		float deltaZ = (rand() % 100) / 100.f * 3.1459f;
		float deltaX = (rand() % 100) / 100.f * 3.1459f;
		
		VS_cb vscb;
		vscb.transform = XMMatrixTranspose(
							XMMatrixRotationZ(angle + deltaZ) *
							XMMatrixRotationX(angle + deltaX) *
							XMMatrixScaling(scale, scale, scale) *
							XMMatrixTranslation(x, y, z) *
							XMMatrixPerspectiveLH(1.f, 9.f / 16.f, 0.5f, 10.f));

		vcbo->Update(m_Window.GetGfx(), vscb);
		m_Window.GetGfx().DrawIndexed((unsigned int)ibo->GetCount());
	}
	
	m_Window.GetGfx().Present();
}