#include <imgui_app_fw.h>

int main(int, char**)
{
	return mu::leaf::try_handle_all(
		[&]() -> mu::leaf::result<int> {
			if (imgui_app_fw()->select_platform())
			{
				if (imgui_app_fw()->init())
				{
					imgui_app_fw()->set_window_title("Hello!");

					// Our state
					bool   show_demo_window	   = true;
					bool   show_another_window = false;
					ImVec4 clear_color		   = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

					while (imgui_app_fw()->pump())
					{
						imgui_app_fw()->begin_frame();

						// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
						if (show_demo_window)
						{
							ImGui::ShowDemoWindow(&show_demo_window);
						}

						// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
						{
							static float f		 = 0.0f;
							static int	 counter = 0;

							ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

							ImGui::Text("This is some useful text.");		   // Display some text (you can use a format strings too)
							ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
							ImGui::Checkbox("Another Window", &show_another_window);

							ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			// Edit 1 float using a slider from 0.0f to 1.0f
							ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

							if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
							{
								counter++;
							}

							ImGui::SameLine();
							ImGui::Text("counter = %d", counter);

							ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
							ImGui::End();
						}

						// 3. Show another simple window.
						if (show_another_window)
						{
							ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
							ImGui::Begin(
								"Another Window",
								&show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

							ImGui::GetWindowDrawList()->AddCallback(
								[](const ImDrawList* parent_list, const ImDrawCmd* cmd) -> void {
									auto user_data = imgui_app_fw_interface::mutable_userdata::get_from_draw_cmd(cmd);
									auto pos_x	   = cmd->ClipRect.x;
									auto width	   = (cmd->ClipRect.z - cmd->ClipRect.x);
									auto pos_y	   = cmd->ClipRect.y;
									auto height	   = (cmd->ClipRect.w - cmd->ClipRect.y);
								},
								nullptr);

							ImGui::Text("Hello from another window!");
							if (ImGui::Button("Close Me"))
							{
								show_another_window = false;
							}

							ImGui::End();
						}

						imgui_app_fw()->end_frame(clear_color);
					}

					imgui_app_fw()->destroy();
				}
			}
			return 0;
		},
		[](mu::leaf::error_info const& unmatched) {
			//  "Unknown failure detected" << std::endl <<
			//  "Cryptic diagnostic information follows" << std::endl <<
			//  unmatched;
			return -1;
		});
}
