#include "FileWindow.h"
#include "Src/Engine/Systems/Graphics/Compiler/AnimationCompiler.h"
#include "Src/Engine/Core/Manager/AssetManager.h"

#include "Src/Events/Events.h"

static std::filesystem::path s_AssetsPath = "Assets";

Eos::FileWindow::FileWindow() :
	path{ "Assets" },
	m_CurrentDirectory{ s_AssetsPath }
{
	m_DirectoryIcon = Texture2D::Create("Buttons/FolderIcon.png");
	m_FileIcon = Texture2D::Create("Buttons/FileIcon_General.png");
	m_SkeletonIcon = Texture2D::Create("Buttons/skull2.png");
	m_MeshIcon = Texture2D::Create("Buttons/mesh.png");
	m_AnimationIcon = Texture2D::Create("Buttons/Animation.png");
	m_ScriptIcon = Texture2D::Create("Buttons/Cscript.png");
}

void Eos::FileWindow::Update()
{
	ImGui::Begin("Content Browser", &active);


	// Going backwards functionality
	if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath))
	{
		if (ImGui::Button("<-"))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}
	}

	ImGui::SameLine();

	// Button for Importing Textures / Static Meshes / Animations / Fonts
	if (ImGui::Button("Import Animation Asset"))
	{
		std::optional<std::string> open_file_path;
		open_file_path = DialogOpenFile(L"Import from...", L"Assets/");

		// Valid path has been chosen
		if (open_file_path.has_value())
		{
			std::string fileName = FileManager::GetFileNameWithoutExtension(open_file_path.value());
			std::string extension = FileManager::GetExtention(open_file_path.value());

			// Run compiler for corresponding filetype
			// Fonts - font compiler
			// Animated mesh - animation compiler
			if (extension == ".fbx")
			{
				// TODO: open up mini window and enable user to choose options on what details to import
				AnimationCompiler::readFBX_to_ownFormat(open_file_path.value());

				std::string meshPath = "Assets/Animations/" + fileName + ".eosmesh";
				std::string skeletonPath = "Assets/Animations/" + fileName + ".eosskeleton";
				std::string animationPath = "Assets/Animations/" + fileName + ".eosanimation";
				AnimationCompiler::readOwnFormat(meshPath, skeletonPath, animationPath);

				PE_CORE_TRACE("Loading and saving completed");
			}
			else if (extension == ".ttf")
			{
				// Font compiler stuff
			}
			else if (extension == ".png")
			{
				// Image compilter stuff
			}
			else
			{
				PE_CORE_WARN("Extension of {} not supported", extension);
			}
		}
		else // invalid path selected
		{
			PE_CORE_WARN("Asset Import Process Stopped");
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Import Static Mesh"))
	{
		std::optional<std::string> open_file_path;
		open_file_path = DialogOpenFile(L"Import from...", L"Assets/");

		// Valid path has been chosen
		if (open_file_path.has_value())
		{
			std::string fileName = FileManager::GetFileNameWithoutExtension(open_file_path.value());
			std::string extension = FileManager::GetExtention(open_file_path.value());

			if (extension == ".fbx")
			{
				// Copy file from source directory into project directory
				std::filesystem::path sour = open_file_path.value();
				std::filesystem::path dest = "Assets/Static_Meshes";
				std::filesystem::copy(sour, dest, std::filesystem::copy_options::overwrite_existing);

				// Save into asset manager
				std::string filepath = open_file_path.value();
				AssetManager::GetInstance().LoadStoreStaticMesh(filepath);

				PE_CORE_TRACE("Loading and Saving completed");
			}
		}
		else // invalid path selected
		{
			PE_CORE_WARN("Import Process Stopped");
		}
	}

	// Button to move to asset folder
	if (ImGui::Button("Assets Path"))
	{
		s_AssetsPath = "Assets";
		m_CurrentDirectory = "Assets";
	}

	ImGui::SameLine();

	// Button for importing scripts
	if (ImGui::Button("Scripts Path"))
	{
		s_AssetsPath = "EosScriptCore/EosScriptCore/Scripts";
		m_CurrentDirectory = "EosScriptCore/EosScriptCore/Scripts";
	}

	ImGui::SameLine();

	std::string number = std::to_string(Eos::AssetManager::GetInstance().Num_StoredMesh());
	ImGui::Text("Meshes Loaded: ");
	ImGui::SameLine();
	ImGui::Text(number.c_str());

	ImGui::SameLine();

	number = std::to_string(Eos::AssetManager::GetInstance().Num_StoredSkeleton());
	ImGui::Text("Skeletons Loaded: ");
	ImGui::SameLine();
	ImGui::Text(number.c_str());

	ImGui::SameLine();

	number = std::to_string(Eos::AssetManager::GetInstance().Num_StoredAnimation());
	ImGui::Text("Animations Loaded: ");
	ImGui::SameLine();
	ImGui::Text(number.c_str());

	ImGui::SameLine();

	number = std::to_string(Eos::AssetManager::GetInstance().Num_StoredStaticMeshes());
	ImGui::Text("Static Meshes Loaded: ");
	ImGui::SameLine();
	ImGui::Text(number.c_str());

	ImGui::NewLine();

	static std::filesystem::path to_delete_file;

	// Settings for the content browser
	static float padding = 8.0f;
	static float thumbnailSize = 64.0f;
	float cellSize = thumbnailSize + padding;
	float panelWidth = ImGui::GetContentRegionAvail().x;

	// Calculating number of items in the columns, minimum 1 column
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;
	ImGui::Columns(columnCount, 0, false);

	// Going through each item in the current directory
	// Can be folder or files
	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& _path = directoryEntry.path();
		auto relativePath = std::filesystem::relative(_path, s_AssetsPath);
		std::string filenameString = relativePath.filename().string();

		// Display image icons
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.82, 0.82, 0.82, 1.0));

		// if a folder
		if (directoryEntry.is_directory())
		{
			ImGui::ImageButton((ImTextureID)m_DirectoryIcon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });
		}
		else if (relativePath.extension() == ".eosmesh")
		{
			if (ImGui::ImageButton((ImTextureID)m_MeshIcon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 }))
			{
				SetAnimationPlayingEvent animEvent{ Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()) };
				PE_DEBUG("Set animation playing {}", Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()));
				PE_PUBLISH_EVENT(animEvent);
			}
		}
		else if (relativePath.extension() == ".eosskeleton")
		{
			if (ImGui::ImageButton((ImTextureID)m_SkeletonIcon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 }))
			{
				SetAnimationPlayingEvent animEvent{ Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()) };
				PE_DEBUG("Set animation playing {}", Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()));
				PE_PUBLISH_EVENT(animEvent);
			}
		}
		else if (relativePath.extension() == ".eosanimation")
		{
			if (ImGui::ImageButton((ImTextureID)m_AnimationIcon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 }))
			{
				SetAnimationPlayingEvent animEvent{ Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()) };
				PE_DEBUG("Set animation playing {}", Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()));
				PE_PUBLISH_EVENT(animEvent);
			}
		}
		else if (relativePath.extension() == ".cs")
		{
			ImGui::ImageButton((ImTextureID)m_ScriptIcon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });
		}
		else
		{
			ImGui::ImageButton((ImTextureID)m_FileIcon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });
		}
		ImGui::PopStyleColor();

		// Navigating through different folders
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
			{
				m_CurrentDirectory /= _path.filename();
			}
		}

		// Right click, Able to delete any file that is not a directory
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && !directoryEntry.is_directory())
		{
			ImGui::OpenPopup("Delete window");
			to_delete_file = relativePath;
		}

		// Show filename under icon
		ImGui::TextWrapped(filenameString.c_str());

		// Put next icon on the next column
		ImGui::NextColumn();
	}

	//// https://github.com/ocornut/imgui/issues/6426
	if (ImGui::BeginPopup("Delete window"))
	{
		if (ImGui::Selectable("  Delete Asset\t\t"))
		{
			// Only if the deletion from disk is successful, then we remove the asset in memeory
			// Will avoid having mismatch where is asset exists in memory but not on disk
			// Remove from local storage
			std::filesystem::path mod_path = s_AssetsPath / to_delete_file;
			if (std::remove(mod_path.string().c_str()) == 0)
			{
				PE_INFO("{} deletion from disk successful", to_delete_file.string());

				// Remove from asset manager
				std::string file_name = Eos::FileManager::GetFileNameWithoutExtension(to_delete_file.string());
				AssetManager::GetInstance().RemoveStaticMesh(file_name.c_str());

				PE_INFO("{} deletion from memory successful", to_delete_file.string());
			}
			else
			{
				PE_WARN("Something went wrong when deleting {} from disk, ABORTING", to_delete_file.string());
			}
		}
		ImGui::EndPopup();
	}

	ImGui::Columns(1);

	ImGui::SliderFloat("Thumbnail size", &thumbnailSize, 16, 512);
	ImGui::SliderFloat("Padding", &padding, 0, 32);
	ImGui::End();
}
