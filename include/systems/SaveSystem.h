#pragma once

#include <string>
#include <cstdint>

// SaveData chứa toàn bộ dữ liệu persistent của người chơi.
// Thêm field mới ở đây khi cần lưu thêm thông tin (ví dụ: settings,
// unlocked content, character progression...).
struct SaveData
{
    int high_score = 0;

    // Thống kê — nền tảng cho các feature tương lai (achievements,
    // stats screen...). Chi phí thêm gần như bằng 0 ngay từ bây giờ.
    int total_runs_played = 0;

    // Skin đã chọn ở MenuScene — dùng cho F5 (character selection).
    // Lưu ở đây để không phải tạo thêm 1 file save riêng sau này.
    std::string selected_skin = "MaleSkin1";
};

// SaveSystem có đúng một trách nhiệm: đọc/ghi SaveData xuống đĩa
// dưới dạng JSON. Không biết gì về gameplay, Player, hay Score.
//
// Định dạng JSON được serialize/parse thủ công (không dùng thư viện
// ngoài) vì schema cố định và đơn giản — tránh thêm dependency vcpkg
// chỉ để đọc vài field phẳng. Nếu sau này SaveData cần cấu trúc lồng
// nhau phức tạp (nested objects, arrays), nên thay bằng nlohmann/json.
class SaveSystem
{
public:

    SaveSystem() = default;

    // Đọc file save. Nếu file không tồn tại hoặc parse lỗi,
    // trả về SaveData với giá trị mặc định (không throw, không crash).
    SaveData load() const;

    // Ghi SaveData xuống file, tự tạo thư mục save/ nếu chưa có.
    // Trả về false nếu ghi thất bại (ví dụ: không có quyền ghi).
    bool save(const SaveData& data) const;

private:

    std::string getFilePath() const;

    bool ensureSaveDirectoryExists() const;

private:

    // Lưu tại thư mục "save/" cạnh assets/, không lưu trong assets/
    // vì assets/ là read-only shipped content theo quy ước ResourceManager.
    static constexpr const char* kSaveDir  = "save";
    static constexpr const char* kFileName = "save/savedata.json";
};