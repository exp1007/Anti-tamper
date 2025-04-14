#include "UI.h"
#include "Globals.h"
#include "../Utils/Utils.h"

#include <string>
#include <vector>
#include <algorithm>

bool ViewAlerts = false;
bool ViewLogs = false;

int UI::Components::MainWindow() {

	ImGuiStyle& Style = ImGui::GetStyle();

	ImGui::SetNextWindowPos({0,0}, ImGuiCond_Always);
	ImGui::SetNextWindowSize(Globals::ClientSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

	ImGui::Begin("Main window", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);

	static bool tempBool = false;

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Alerts", NULL, &ViewAlerts);
			ImGui::MenuItem("Logs", NULL, &ViewLogs);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("UI Framework")) {
			ImGui::MenuItem("Demo window", NULL, &tempBool);
			ImGui::MenuItem("Styles window", NULL, &tempBool);
			ImGui::EndMenu();
		}
	}
	ImGui::EndMenuBar();

	// Childs
	ImVec2 ChildSize = ImGui::GetWindowSize();
	ChildSize.x = ChildSize.x / 2 - Style.WindowPadding.x - Style.ItemSpacing.x / 2;
	ChildSize.y = ChildSize.y / 2 - Style.WindowPadding.y * 2;

	if (ImGui::BeginChild("First child", { ChildSize.x,0 }, ImGuiChildFlags_Border)) {
		static std::vector<Utils::ProcessEntity> ProcessList;

		ImGui::Checkbox("Enable protection", &tempBool);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 14, 4 });

		if (ImGui::Button("Select process")) {
			ImGui::OpenPopup("Process list popup");
			ProcessList = Utils::GetProcessList();
		}

		ImGui::SetNextWindowSizeConstraints({ 0,0 }, { FLT_MAX,500 });
		if (ImGui::BeginPopup("Process list popup")) {
			static char SearchBuff[255] = "";
			ImGui::InputText("##SearchProcess", SearchBuff, IM_ARRAYSIZE(SearchBuff));
			std::string StrSearchBuff(SearchBuff);

			if (ImGui::BeginListBox("##Process list")) {
				for (auto& Proc : ProcessList) {
					// Filter
					if (StrSearchBuff.size() > 0) {
						if (Utils::StrToLower(Proc.Name).find(Utils::StrToLower(StrSearchBuff)) == std::string::npos)
							continue;
					}

					std::string FormatedProc;
					FormatedProc += std::to_string(Proc.PID) + " | " + Proc.Name;
					bool IsSelected = false;
					ImGui::Selectable(FormatedProc.c_str(), IsSelected);

				}

				ImGui::EndListBox(); 
			}

			if(ImGui::Button("Refresh"))
				ProcessList = Utils::GetProcessList();

			ImGui::EndPopup();
		}

		ImGui::PopStyleVar();
		ImGui::EndChild();
	}

	ImGui::SameLine();

	if (ImGui::BeginChild("Second child", { ChildSize.x,0 }, ImGuiChildFlags_Border)) {

		// Alerts terminal
		if (ViewAlerts) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 14, 4 });
			ImGui::Text("Alerts");
			ImGui::BeginChild("Alerts terminal", { 0,200 }, ImGuiChildFlags_FrameStyle);
			ImGui::Text("Detected memory manipulation");
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}

		// Logs terminal
		if (ViewLogs) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 14, 4 });
			ImGui::Text("Logs terminal");
			ImGui::BeginChild("Logs terminal", { 0,200 }, ImGuiChildFlags_FrameStyle);
			ImGui::Text("--- Start of logs ---");
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}

		ImGui::EndChild();
	}

	ImGui::End();

	// Cleaning pushes
	ImGui::PopStyleVar();

	return 1;
}