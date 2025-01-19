#pragma once

#include <glm\glm.hpp>

#include "Src\Engine\Core\Interfaces\IComponent.h"
#include "Src\Engine\Systems\Graphics\Shader.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\TestModel.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\TestShader.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\EosEditorResources.h"

#define GET_MESHES_MAP std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS))->GetMeshesMap()

//enum  MeshType : int
//{
//	SPHERE = 0,
//	CUBE = 1,
//	NONE = 2,
//};
//
//enum  ShaderType : int
//{
//	PHONG = 0,
//	SHADOW = 1,
//	DEFAULT = 2,
//};


class Mesh_Component : public IComponent
{
public:

	MeshType type{ MeshType::NONE };
	ShaderType sType{ ShaderType::DEFAULT };

	Test::Model _model_choice;
	Test::Shader _shader_choice;
	Mesh_Component();
	~Mesh_Component();

	virtual void Init() override;

	property_vtable()
};
property_begin(Mesh_Component)
{
  property_var(type)
  , property_var(sType)
  , property_var(_model_choice.directory)
}property_vend_h(Mesh_Component)
