#pragma once

#include <imgui.h>
#include <framegraph/FG.h>
#include <mu_stdlib.h>

struct imgui_app_fw_interface
{
	imgui_app_fw_interface()		  = default;
	virtual ~imgui_app_fw_interface() = default;

	virtual bool select_platform()					 = 0;
	virtual void set_window_title(const char* title) = 0;
	virtual bool init()								 = 0;
	virtual bool pump()								 = 0;
	virtual void begin_frame()						 = 0;
	virtual void end_frame(ImVec4 clear_color)		 = 0;
	virtual void destroy()							 = 0;

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