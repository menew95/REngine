#include <Editor_pch.h>

#include <editor\GUI\Document.h>

namespace editor
{
	Document::Document(std::string name)
	: m_documentName(name)
	{

	}

	Document::~Document()
	{
		
	}

	void Document::Begin()
	{
		ImGui::Begin(m_documentName.c_str(), nullptr, m_flags);
	}

	void Document::Render()
	{

	}

	void Document::End()
	{
		ImGui::End();
	}
}