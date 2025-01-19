#include "AssetManager.h"

#include "Src/pch.h"


#include <tuple>

/*****************************************************************************
Font Assets
******************************************************************************/
void Eos::FontAsset::GetFontCharUVBounds(uint32_t unicodeChar, float& out_l, float& out_r, float& out_b, float& out_t)
{
	if (fontUVBounds.count(unicodeChar) > 0)
	{
		out_l = std::get<0>(fontUVBounds[unicodeChar]);
		out_r = std::get<1>(fontUVBounds[unicodeChar]);
		out_b = std::get<2>(fontUVBounds[unicodeChar]);
		out_t = std::get<3>(fontUVBounds[unicodeChar]);
	}
	else
	{
		// Can consider adding unicodeChar and atlas to the error message
		PE_WARN("GetFontCharUVBounds(): Font does not contain character");
	}
}

void Eos::FontAsset::GetFontCharQuadBounds(uint32_t unicodeChar, float& out_l, float& out_r, float& out_b, float& out_t, uint32_t prevChar)
{
	if (fontQuadBounds.count(unicodeChar) > 0)
	{
		out_l = std::get<0>(fontQuadBounds[unicodeChar]);
		out_r = std::get<1>(fontQuadBounds[unicodeChar]);
		out_b = std::get<2>(fontQuadBounds[unicodeChar]);
		out_t = std::get<3>(fontQuadBounds[unicodeChar]);

		if (fontKerns.count(std::pair(unicodeChar, prevChar)) > 0)
		{
			out_l += fontKerns[std::pair(unicodeChar, prevChar)];
		}
	}
	else
	{
		PE_WARN("GetFontCharQuadBounds(): Font does not contain character");
	}
}

double Eos::FontAsset::GetFontCharAdvance(uint32_t unicodeChar)
{
	if (fontAdvances.count(unicodeChar) > 0)
	{
		return fontAdvances[unicodeChar];
	}
	else
	{
		PE_WARN("Graphics::GetFontCharAdvance(): Font does not contain character");
		return 0.0;
	}
}



namespace Eos
{
	// Definition of Initialize Asset Manager
	// Will comb through selected file directories and save files into library
	void AssetManager::Init()
	{
		// Loadup Assets from a predetermined file when startup
		std::string path{ "Assets/Animations/" };
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string filepath = entry.path().string();

			// Get extension
			std::string extension = Eos::FileManager::GetExtention(filepath);
			if (extension == ".eosmesh")
			{
				// Find corresponding skeleton file
				std::string basefilename = Eos::FileManager::GetFileNameWithoutExtension(filepath);
				std::string skeletonfilename = path + "/" + basefilename + ".eosskeleton";

				// Run animation compiler
				AnimationCompiler::read_mesh_skeleton_from_file(filepath, skeletonfilename);
			}
			else if (extension == ".eosanimation")
			{
				AnimationCompiler::read_animation_from_file(filepath);
			}
			else if (extension == ".eosskeleton")
			{

			}
			else
			{
				PE_WARN("Unrecognized file type found in Assets/Animations");
			}
		}

		// Load Static Meshes from a predetermined file when startup
		path = "Assets/Static_Meshes";
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string filepath = entry.path().string();

			// Get extension
			std::string extension = Eos::FileManager::GetExtention(filepath);
			if (extension == ".fbx")
			{
				LoadStoreStaticMesh(filepath);
			}
			else
			{
				PE_WARN("Unrecognized file type found in Assets/Static_Meshes");
			}
		}
	}

	void AssetManager::LoadStoreAnimations(std::string& name)
	{

	}

	void AssetManager::LoadStoreStaticMesh(std::string& filepath)
	{
		Test::Model model;
		model.Init(filepath);
		std::string rawname = Eos::FileManager::GetFileNameWithoutExtension(filepath);
		StoreStaticMesh(rawname.c_str(), model);
	}

	void AssetManager::StoreFont(const char* name, const char* filepath)
	{
		// Run filepath through font compiler
		if (!_fontLibrary.count(name))
		{
			//_fontLibrary[name] = font;
		}
		else
		{

		}
	}

	FontAsset AssetManager::GetFont(const char* name)
	{
		try
		{
			return _fontLibrary.at(name);
		}
		catch (std::out_of_range& e)
		{
			PE_WARN("Unable to get Font {}", e.what());
		}
		catch (...)
		{
			PE_WARN("Unknown exception caught");
		}
	}

	void AssetManager::StoreAnimation(const char* name, AnimationCompiler::AnimationTrack info)
	{
		// Check if the entry already exists
		if (!_animationLibrary.count(name))
		{
			std::string rawname = FileManager::GetFileNameWithoutExtension(name);
			_animationLibrary[rawname.c_str()] = info;
			PE_CORE_DEBUG("Animation {} has been addeded to asset manager", rawname);
		}
		else
		{
			PE_WARN("Animation of name {} already exists and has been imported and saved into assets", name);
		}
	}

	AnimationCompiler::AnimationTrack* AssetManager::GetAnimation(const char* name)
	{
		try
		{
			return &_animationLibrary.at(name);
		}
		catch (std::out_of_range& e)
		{
			PE_WARN("Unable to get Animation {}, {}, does not exist", name, e.what());
		}
		catch (...)
		{
			PE_WARN("Unknown exception caught");
		}
		return nullptr;
	}

	void AssetManager::StoreMesh(const char* name, AnimationCompiler::MeshInfo info)
	{
		if (!_meshLibrary.count(name))
		{
			std::string rawname = FileManager::GetFileNameWithoutExtension(name);
			_meshLibrary[rawname.c_str()] = info;
			PE_CORE_DEBUG("Mesh {} has been added to asset manager", rawname);
		}
		else
		{
			PE_WARN("Mesh of name {} already exists and has been imported and saved into assets", name);
		}
	}

	AnimationCompiler::MeshInfo* AssetManager::GetMesh(const char* name)
	{
		try
		{
			return &_meshLibrary.at(name);
		}
		catch (std::out_of_range& e)
		{
			PE_WARN("Unable to get Mesh {}, {}, does not exist", name, e.what());
		}
		catch (...)
		{
			PE_WARN("Unknown exception caught");
		}
		return nullptr;
	}

	void AssetManager::StoreSkeleton(const char* name, AnimationCompiler::SkeletonInfo info)
	{
		if (!_skeletonLibrary.count(name))
		{
			std::string rawname = FileManager::GetFileNameWithoutExtension(name);
			_skeletonLibrary[rawname.c_str()] = info;
			PE_CORE_DEBUG("Skeleton {} has been added to asset manager", rawname);
		}
		else
		{
			PE_WARN("Skeleton of name {} already exists and has been imported and saved into assets", name);
		}
	}

	AnimationCompiler::SkeletonInfo* AssetManager::GetSkeleton(const char* name)
	{
		try
		{
			return &_skeletonLibrary.at(name);
		}
		catch (std::out_of_range& e)
		{
			PE_WARN("Unable to get Skeleton {}, {}, does not exist", name, e.what());
		}
		catch (...)
		{
			PE_WARN("Unknown exception caught");
		}
		return nullptr;
	}

	void AssetManager::StoreStaticMesh(const char* name, Test::Model info)
	{
		if (!_modelLibrary.count(name))
		{
			std::string rawname = FileManager::GetFileNameWithoutExtension(name);
			_modelLibrary[rawname.c_str()] = info;
			PE_CORE_DEBUG("Static Mesh {} has been added to asset manager", rawname);
		}
		else
		{
			PE_WARN("Static Mesh of name {} already exists and has been imported and saved into assets", name);
		}
	}

	Test::Model* AssetManager::GetStaticMesh(const char* name)
	{
		try
		{
			return &_modelLibrary.at(name);
		}
		catch (std::out_of_range& e)
		{
			PE_WARN("Unable to get Static Mesh {}, {}, does not exist", name, e.what());
		}
		catch (...)
		{
			PE_WARN("Unknown exception caught");
		}
		return nullptr;
	}

	void AssetManager::RemoveStaticMesh(const char* name)
	{
		if (_modelLibrary.erase(name) == 0)
		{
			PE_WARN("{} was not erased, likely not stored at first", name);
		}
		else
		{
			PE_INFO("{} was erased successfully", name);
		}
	}

	std::vector<std::string> AssetManager::GetStoredStaticMeshNames()
	{
		int count = 0;
		std::vector<std::string> keys(_modelLibrary.size());
		for (const std::pair<std::string, Test::Model>& pair : _modelLibrary)
		{
			keys[count] = pair.first;
			count++;
		}
		return keys;
	}

	int AssetManager::Num_StoredAnimation() { return (int)_animationLibrary.size(); }

	int AssetManager::Num_StoredMesh() { return (int)_meshLibrary.size(); }

	int AssetManager::Num_StoredSkeleton() { return (int)_skeletonLibrary.size(); }

	int AssetManager::Num_StoredStaticMeshes() { return (int)_modelLibrary.size(); }
}
