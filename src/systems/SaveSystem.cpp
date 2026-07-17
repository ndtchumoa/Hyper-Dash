#include "systems/SaveSystem.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

namespace
{
    // Trim khoảng trắng đầu/cuối string.
    std::string trim(const std::string& s)
    {
        const auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
            return "";

        const auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    // Escape các ký tự đặc biệt tối thiểu cho JSON string value.
    std::string jsonEscape(const std::string& s)
    {
        std::string out;
        out.reserve(s.size());

        for (char c : s)
        {
            if (c == '"' || c == '\\')
                out += '\\';

            out += c;
        }

        return out;
    }

    // Parse một dòng dạng "key": value, hoặc "key": "value",
    // Trả về true nếu parse thành công, ghi kết quả vào key/rawValue.
    bool parseLine(
        const std::string& line,
        std::string&       key,
        std::string&       rawValue)
    {
        const std::string trimmed = trim(line);

        if (trimmed.empty() || trimmed[0] != '"')
            return false;

        const auto keyEnd = trimmed.find('"', 1);
        if (keyEnd == std::string::npos)
            return false;

        key = trimmed.substr(1, keyEnd - 1);

        const auto colon = trimmed.find(':', keyEnd);
        if (colon == std::string::npos)
            return false;

        std::string value = trim(trimmed.substr(colon + 1));

        // Bỏ dấu phẩy cuối dòng nếu có.
        if (!value.empty() && value.back() == ',')
            value.pop_back();

        rawValue = trim(value);
        return true;
    }

    // Parse rawValue là JSON string ("...") -> trả về nội dung bên trong.
    // Nếu không phải string (không có dấu ngoặc kép bao quanh), trả về "".
    std::string parseStringValue(const std::string& rawValue)
    {
        if (rawValue.size() >= 2 &&
            rawValue.front() == '"' &&
            rawValue.back()  == '"')
        {
            return rawValue.substr(1, rawValue.size() - 2);
        }

        return "";
    }

    int parseIntValue(const std::string& rawValue)
    {
        try
        {
            return std::stoi(rawValue);
        }
        catch (...)
        {
            return 0;
        }
    }
}

SaveData SaveSystem::load() const
{
    SaveData data; // giá trị mặc định nếu không load được

    const std::string path = getFilePath();

    std::ifstream file(path);

    if (!file.is_open())
    {
        // File chưa tồn tại — bình thường ở lần chạy đầu tiên.
        std::cout
            << "[SaveSystem] No save file found at " << path
            << " — using defaults.\n";
        return data;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::string key, rawValue;

        if (!parseLine(line, key, rawValue))
            continue;

        if (key == "high_score")
        {
            data.high_score = parseIntValue(rawValue);
        }
        else if (key == "total_runs_played")
        {
            data.total_runs_played = parseIntValue(rawValue);
        }
        else if (key == "selected_skin")
        {
            const std::string s = parseStringValue(rawValue);
            if (!s.empty())
                data.selected_skin = s;
        }
        // Field lạ (từ version save cũ/mới hơn) bị bỏ qua an toàn —
        // không làm hỏng toàn bộ quá trình load.
    }

    std::cout
        << "[SaveSystem] Loaded: high_score=" << data.high_score
        << ", total_runs_played=" << data.total_runs_played
        << ", selected_skin=" << data.selected_skin << '\n';

    return data;
}

bool SaveSystem::save(const SaveData& data) const
{
    if (!ensureSaveDirectoryExists())
    {
        std::cerr
            << "[SaveSystem] Failed to create save directory.\n";
        return false;
    }

    const std::string path = getFilePath();

    std::ofstream file(path, std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr
            << "[SaveSystem] Failed to open file for writing: "
            << path << '\n';
        return false;
    }

    file << "{\n";
    file << "  \"high_score\": "        << data.high_score        << ",\n";
    file << "  \"total_runs_played\": " << data.total_runs_played << ",\n";
    file << "  \"selected_skin\": \""   << jsonEscape(data.selected_skin) << "\"\n";
    file << "}\n";

    if (!file.good())
    {
        std::cerr << "[SaveSystem] Write error to " << path << '\n';
        return false;
    }

    std::cout << "[SaveSystem] Saved to " << path << '\n';

    return true;
}

std::string SaveSystem::getFilePath() const
{
    return kFileName;
}

bool SaveSystem::ensureSaveDirectoryExists() const
{
    std::error_code ec;

    if (std::filesystem::exists(kSaveDir, ec))
        return true;

    return std::filesystem::create_directories(kSaveDir, ec);
}