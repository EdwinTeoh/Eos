#pragma once

#include "Tools/Singleton.h"
//#include "Src/Engine/Systems/Graphics/Platform/OpenGLTexture.h"
#include "Src/Engine/Systems/Graphics/Compiler/AnimationCompiler.h"
#include "Src/Engine/Systems/Graphics/Compiler/FontCompiler.h"
#include "Src/Engine/Systems/Graphics/EosRenderer/TestModel.h"

#include <string>
#include <unordered_map>
#include <memory>

class Texture2D;

namespace Eos
{
	class FontAsset
	{
		// Metrics
		std::map<int, uint32_t> indexToCodePoint;
		std::map<uint32_t, double> fontAdvances;
		std::map<uint32_t, std::tuple<float, float, float, float>> fontUVBounds;
		std::map<uint32_t, std::tuple<float, float, float, float>> fontQuadBounds;
		std::tuple<double, double, double> fontVerticalMetrics;
		std::map<std::pair<uint32_t, uint32_t>, double> fontKerns;

	public:
		// Image atlas
		std::shared_ptr<Texture2D> atlas;

		void GetFontCharUVBounds(uint32_t unicodeChar, float& out_l, float& out_r, float& out_b, float& out_t);
		void GetFontCharQuadBounds(uint32_t unicodeChar, float& out_l, float& out_r, float& out_b, float& out_t, uint32_t prevChar = 0);
		double GetFontCharAdvance(uint32_t unicodeChar);
	};

	/*
	Definition of a Animation
	*/
	//class AnimationAsset
	//{
	//
	//public:
	//	AnimationCompiler::Ex_Info extractedInfo;
	//
	//	AnimationCompiler::AnimationTrack animation;	// Actual info
	//};

	/*
	Definition of a Skeleton
	*/
	//class SkeletonAsset
	//{
	//public:
	//	// Bone count
	//	// Bone info
	//};

	/*
	Definition of a mesh for animation
	*/
	//class AnimatedMeshAsset
	//{
	//public:
	//
	//};


	/*
	Asset Mananger class
	*/
	class AssetManager : public Singleton<AssetManager>
	{
		// Stored font library
		std::unordered_map<const char*, FontAsset> _fontLibrary;

		// Animation Library
		std::unordered_map<std::string, AnimationCompiler::AnimationTrack> _animationLibrary;

		// Mesh Library
		std::unordered_map<std::string, AnimationCompiler::MeshInfo> _meshLibrary;
		
		// Skeleton library
		std::unordered_map<std::string, AnimationCompiler::SkeletonInfo> _skeletonLibrary;

		// Static Mesh Library
		std::unordered_map<std::string, Test::Model> _modelLibrary;

	public:
		// Initialize Asset Manager
		void Init();

		// Loading and Storing assets called from editor
		void LoadStoreAnimations(std::string& name);
		void LoadStoreStaticMesh(std::string& name);

		// Getter and Loader for fonts
		void StoreFont(const char* name, const char* filepath);
		FontAsset GetFont(const char* name);

		// Getter and Storer for animations
		void StoreAnimation(const char* name, AnimationCompiler::AnimationTrack);
		AnimationCompiler::AnimationTrack* GetAnimation(const char* name);

		// Getter and Storer for meshes
		void StoreMesh(const char* name, AnimationCompiler::MeshInfo);
		AnimationCompiler::MeshInfo* GetMesh(const char* name);

		// Getter and Storer for skeletons
		void StoreSkeleton(const char* name, AnimationCompiler::SkeletonInfo);
		AnimationCompiler::SkeletonInfo* GetSkeleton(const char* name);

		// Getter and Storer for static meshes
		void StoreStaticMesh(const char* name, Test::Model);
		Test::Model* GetStaticMesh(const char* name);
		void RemoveStaticMesh(const char* name);
		std::vector<std::string> GetStoredStaticMeshNames();

		int Num_StoredAnimation();
		int Num_StoredMesh();
		int Num_StoredSkeleton();
		int Num_StoredStaticMeshes();
	};
}
