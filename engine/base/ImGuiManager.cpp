#include "ImGuiManager.h"

void ImGuiManager::Initialize(WinApp* winApp_, DirectXCommon* directXcommon_)
{
	winApp = winApp_;
	directXcommon = directXcommon_;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	ImGui_ImplDX12_Init(
		directXcommon->GetDevice(),
		directXcommon->GeSwapChainDesc().BufferCount,
		directXcommon->GetRtvDesc().Format,
		directXcommon->GetSrvDescriptorHeap(),
		directXcommon->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		directXcommon->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart()
	);
}

void ImGuiManager::BeginFlame(DirectXCommon* directXcommon_)
{
	directXcommon = directXcommon_;
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ID3D12DescriptorHeap* descripterHeap[] = { directXcommon->GetSrvDescriptorHeap() };
	directXcommon->GetCommandList()->SetDescriptorHeaps(1, descripterHeap);
	
}

void ImGuiManager::EndFlame(DirectXCommon* directXcommon_)
{
	directXcommon = directXcommon_;
	ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXcommon->GetCommandList());
	
	
	

}

void ImGuiManager::Release()
{

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
