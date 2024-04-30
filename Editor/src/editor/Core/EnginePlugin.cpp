#include <Editor_pch.h>
#include <editor\Core\EnginePlugin.h>

#include <common\Module.h>

#include <rengine\rengine_api.h>

std::unique_ptr<Module> g_engineModule;

namespace editor
{
	DEFINE_SINGLETON_CLASS(EnginePlugin, 
		{

		},
		{

		},
		{

		})

	void EnginePlugin::Initialize(WindowInfo& winInfo)
	{
		g_engineModule = Module::Load(Module::GetModuleFilename("REngine").c_str());

		auto _constructor = (rengine::REngine* (*)())g_engineModule->LoadProcedure("CreateREngine");

		g_engineModule->LoadProcedure("QuitREngine");

		m_pRengine = _constructor();

		rengine::REngineDesc _desc;

		_desc._windowInfo = winInfo;

		m_pRengine->Initialize(&_desc);

		((rengine::REngine*)m_pRengine)->SetEngineUpdate(false);
	}

	void EnginePlugin::SetEngineUpdate(bool val)
	{
		((rengine::REngine*)m_pRengine)->SetEngineUpdate(val);
	}
}