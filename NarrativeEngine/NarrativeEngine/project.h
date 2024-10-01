#pragma once

#include "scene.h"

//read project file


//write to project file

#include <shobjidl.h>  // For IFileDialog

class ProjectManager
{
    std::wstring currentProjectFileDirectory;
    std::wstring currentProjectFile;

public:
    void SetCurrentlySelectedObject(std::wstring path)
    {
        currentProjectFile = path;
    }
    std::wstring GetCurrentlySelectedObject()
    {
        return currentProjectFile;
    }
    void SetCurrentProjectDirectory(std::wstring path)
    {
        currentProjectFileDirectory = path;
    }
    std::wstring GetCurrentProjectDirectory() { return currentProjectFileDirectory; }
}manager_project;

inline std::wstring select_directory()
{
    CoInitialize(NULL); // Initialize COM

    IFileDialog* pfd = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

    if (SUCCEEDED(hr))
    {
        // Set the dialog to allow picking folders
        DWORD dwOptions;
        pfd->GetOptions(&dwOptions);
        pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

        // Show the dialog
        hr = pfd->Show(NULL);
        if (SUCCEEDED(hr))
        {
            // Get the result of the dialog (selected folder)
            IShellItem* pItem;
            hr = pfd->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                // Retrieve the file path from the selected item
                PWSTR pszFilePath = NULL;
                pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                // Convert the PWSTR to std::wstring
                std::wstring folderPath = pszFilePath;

                // Clean up
                CoTaskMemFree(pszFilePath);
                pItem->Release();
                pfd->Release();
                CoUninitialize();

                return folderPath;
            }
        }

        pfd->Release();
    }

    CoUninitialize();
    return L"";  // Return an empty string if the user cancels or there's an error
}


void create_subdirectory(const std::wstring& baseDir, const std::wstring& subDir)
{
    std::wstring fullPath = baseDir + L"\\" + subDir;
    if (CreateDirectory(fullPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
    {
        wprintf(L"Directory created: %s\n", fullPath.c_str());
    }
    else
    {
        wprintf(L"Failed to create directory: %s\n", fullPath.c_str());
    }
}

std::wstring dummy()
{
    // Let the user select a directory
    std::wstring folderPath = select_directory();

    if (!folderPath.empty()) {
        wprintf(L"Selected Directory: %s\n", folderPath.c_str());

        // Create the subdirectories
        create_subdirectory(folderPath, L"Project");
        create_subdirectory(folderPath, L"Project/Models");
        create_subdirectory(folderPath, L"Project/Textures");
        create_subdirectory(folderPath, L"Project/Animation");
        create_subdirectory(folderPath, L"Project/Scenes");
        create_subdirectory(folderPath, L"Project/Engine");
    }
    else {
        wprintf(L"Directory selection canceled or failed.\n");
    }
    return folderPath;
}

std::vector<std::wstring> get_files_in_directory(const std::wstring& directoryPath)
{
    std::vector<std::wstring> fileNames;

    // Add a wildcard to the directory path to search for all files
    std::wstring searchPath = directoryPath + L"\\*";

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        wprintf(L"Failed to open directory: %s\n", directoryPath.c_str());
        return fileNames;
    }

    do {
        const std::wstring fileOrDirName = findFileData.cFileName;

        // Ignore "." and ".." directories
        if (fileOrDirName != L"." && fileOrDirName != L"..") {
            fileNames.push_back(fileOrDirName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return fileNames;
}

void print_directory_contents(const std::wstring& directoryPath)
{
    std::vector<std::wstring> fileNames = get_files_in_directory(directoryPath);

    if (!fileNames.empty()) {
        //wprintf(L"Contents of directory %s:\n", directoryPath.c_str());
        for (const auto& fileName : fileNames) {
            std::string selectedName = "";
            std::string selectedObject = convertWStringToString(manager_project.GetCurrentlySelectedObject());
            if (selectedObject != "")selectedName = selectedObject;
            //wprintf(L"%s\n", fileName.c_str());
            std::string objectName = convertWStringToString(fileName.c_str());
            if (ImGui::Selectable(clean_string_for_display(objectName).c_str(), selectedName == objectName)) {
                selectedName = objectName;
                
                manager_project.SetCurrentlySelectedObject(directoryPath+L"\\" + fileName);
                
                std::cout<< convertWStringToString(directoryPath+L"\\" + fileName);
            }
        }
    }
    else {
        ImGui::Text("No files found or directory is empty.\n");
    }
}
