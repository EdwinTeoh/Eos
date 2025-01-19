#pragma once


#include "Src\Engine\Core\Interfaces\IComponent.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\TestModel.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\TestShader.h"

class Static_Mesh_Component : public IComponent
{
	//Test::Mesh mesh; // no default constructor for mesh

public:

	Static_Mesh_Component();
	Static_Mesh_Component(std::string filepath);
	~Static_Mesh_Component();

	std::string directory;

	// TODO: remove these test scopes
	Test::Model* meshptr;

	virtual void Init() override;
	//void LoadModelAndShader(const std::string& filepath);

	property_vtable()
};

property_begin(Static_Mesh_Component)
{
	property_var(directory)
}property_vend_h(Static_Mesh_Component)