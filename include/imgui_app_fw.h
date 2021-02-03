#pragma once

#include <imgui.h>
#include <framegraph/FG.h>
#include <mu_stdlib.h>

struct imgui_app_fw_interface
{
	imgui_app_fw_interface()		  = default;
	virtual ~imgui_app_fw_interface() = default;

	virtual mu::leaf::result<void> select_platform() noexcept					= 0;
	virtual mu::leaf::result<void> set_window_title(const char* title) noexcept = 0;
	virtual mu::leaf::result<void> init() noexcept								= 0;
	virtual mu::leaf::result<bool> pump() noexcept								= 0;
	virtual mu::leaf::result<void> begin_frame() noexcept						= 0;
	virtual mu::leaf::result<void> end_frame(ImVec4 clear_color) noexcept		= 0;
	virtual mu::leaf::result<void> destroy() noexcept							= 0;

	struct mutable_userdata
	{
		mutable_userdata()			= default;
		virtual ~mutable_userdata() = default;

		const FG::CommandBuffer* m_cmdbuf;
		const FG::LogicalPassID	 m_pass_id;
		void*					 m_original_user_data;
		FG::Task				 m_task_result;

		mutable_userdata(const FG::CommandBuffer* cmdbuf, const FG::LogicalPassID pass_id) : m_cmdbuf{cmdbuf}, m_pass_id{pass_id}, m_original_user_data{nullptr} {}

		FG::Task call(const ImDrawList& cmd_list, const ImDrawCmd& cmd)
		{
			m_original_user_data = cmd.UserCallbackData;

			ImDrawCmd tmp		 = cmd;
			tmp.UserCallbackData = this;
			cmd.UserCallback(&cmd_list, &tmp);
			return m_task_result;
		}

		static inline mutable_userdata* get_from_draw_cmd(const ImDrawCmd* cmd)
		{
			return reinterpret_cast<mutable_userdata*>(cmd->UserCallbackData);
		}
	};
};

using imgui_app_fw = mu::exported_singleton<mu::virtual_singleton<imgui_app_fw_interface>>;