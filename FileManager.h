#pragma once

#include <string>
#include <Windows.h>
#include <mmsystem.h>


class FileManager
{
public:
	static const FileManager& getInstance()
	{
		static const FileManager instance;
		return instance;
	}

	const std::wstring& workingDirectory() const { return m_working_directory; }

	const std::wstring& musicDirectory() const { return m_music_directory; }

	const std::wstring& mainAmbienceTheme() const { return m_main_ambience_theme; }

private:
	FileManager()
	{
		// The (absolute) path of the executable
		WCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring buffer_wstring(buffer);
		std::wstring::size_type pos = buffer_wstring.find_last_of(L"\\/");
		m_working_directory = buffer_wstring.substr(0, pos);

		// The (absolute) path of the music files
		m_music_directory = m_working_directory + L"\\Music";

		// The file of the main ambience theme
		m_main_ambience_theme = m_music_directory + L"\\lake_wind_ambience.wav";
	}

public:
	FileManager(const FileManager&) = delete;

	~FileManager() = default;

	FileManager& operator = (const FileManager&) = delete;

private:
	std::wstring m_working_directory;

	std::wstring m_music_directory;

	std::wstring m_main_ambience_theme;
};