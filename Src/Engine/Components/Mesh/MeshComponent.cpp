#include "MeshComponent.h"

Mesh_Component::Mesh_Component() :
	type(MeshType::NONE)
{
}

Mesh_Component::~Mesh_Component()
{
}

void Mesh_Component::Init()
{
	_model_choice = Eos::EosEditorAssets::GetInstance().GetModel(_model_choice.directory);
	_shader_choice.Init("shaders/mesh.vert", "shaders/mesh.frag");
}


