#pragma once

#include <filesystem>
namespace File
{
    using Path = std::filesystem::path;
}


inline static File::Path GetProjectDir()
{
#ifdef PL_WORK_DIR
    return File::Path(PL_WORK_DIR);
#else
    return File::Path(std::filesystem::current_path());
#endif
}
inline static File::Path CastToProjectPath(const File::Path& path)
{
    return GetProjectDir() / path;
}
