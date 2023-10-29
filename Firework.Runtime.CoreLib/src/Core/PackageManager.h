#pragma once

#include "Firework.Runtime.CoreLib.Exports.h"

#include <filesystem>
#include <map>
#include <robin_hood.h>
#include <unordered_map>
#include <Library/Hash.h>
#include <Library/TypeInfo.h>

namespace Firework
{
    namespace Internal
    {
        class CoreEngine;
    }

    namespace PackageSystem
    {
        class BinaryPackageFile;
        class PackageManager;

        struct __firework_corelib_api PackageFile
        {
            struct Reflection {
                uint64_t typeID;
            } reflection;

            std::filesystem::path fileLocalPath;

            virtual ~PackageFile() = 0;

            friend class Firework::PackageSystem::PackageManager;
            friend class Firework::PackageSystem::BinaryPackageFile;
            friend class Firework::Internal::CoreEngine;
        protected:
            inline PackageFile() = default;
        private:
            inline PackageFile(uint64_t typeID, std::filesystem::path localPath) :
            reflection { typeID }, fileLocalPath(std::move(localPath))
            {
            }
        };

        class __firework_corelib_api BinaryPackageFile final : public PackageFile
        {
            std::vector<uint8_t> data;
            
            inline BinaryPackageFile(std::vector<uint8_t> data, std::filesystem::path localPath) :
            PackageFile(__typeid(BinaryPackageFile).qualifiedNameHash(), std::move(localPath)), data(std::move(data))
            {
            }
        public:
            inline const std::vector<uint8_t>& binaryData()
            {
                return this->data;
            }

            friend class Firework::PackageSystem::PackageManager;
        };

        template <typename T>
        inline T* file_cast(PackageFile* file)
        {
            static_assert(std::is_base_of<PackageFile, T>::value, "File cast can only work on type \"PackageFile\"!");
            return (file && file->reflection.typeID == __typeid(T).qualifiedNameHash()) ? static_cast<T*>(file) : nullptr;
        }
        
        enum class Endianness : uint_fast8_t
        {
            Big = 0,
            Little = 1
        };
        class __firework_corelib_api PackageManager final
        {
            static Endianness endianness;

            static std::map<std::streamoff, std::map<size_t, robin_hood::unordered_map<std::basic_string<uint8_t>, PackageFile* (*)(std::vector<uint8_t>)>, std::greater<size_t>>> binFileHandlers;
            static robin_hood::unordered_map<std::wstring, PackageFile* (*)(std::u32string)> textFileHandlers;

            static std::vector<PackageFile*> loadedCorePackage;
            static std::ifstream corePackageStream;
            
            static void normalizePath(std::wstring& path);
            
            static void loadCorePackageIntoMemory(const std::filesystem::path& packagePath);
            static void freeCorePackageInMemory();
        public:
            PackageManager() = delete;

            template <typename PackageFileType>
            inline static void addBinaryFileHandler(const std::vector<uint8_t>& signature, std::streamoff offset = 0)
            {
                static_assert(std::is_final<PackageFileType>::value, "A custom package file type is required to be final.");
                static_assert(std::is_base_of<PackageFile, PackageFileType>::value, "A custom package file type is required to be derived from Firework::PackageSystem::PackageFile.");
                // FIXME: This doesn't work. static_assert(std::is_constructible<PackageFileType, std::vector<uint8_t>>::value, "A custom package file type is required to be constructible from a std::vector<uint8_t>.");

                PackageManager::binFileHandlers[offset][signature.size()].emplace
                (
                    std::basic_string<uint8_t>(signature.data(), signature.size()),
                    [](std::vector<uint8_t> data) -> PackageFile*
                    {
                        PackageFileType* ret = new PackageFileType(std::move(data));
                        ret->reflection.typeID = __typeid(PackageFileType).qualifiedNameHash();
                        return ret;
                    }
                );
            }
            static void removeBinaryFileHandler(const std::vector<uint8_t>& signature, std::streamoff offset);
            // NB: Text files are read as utf-8, but converted to utf-32.
            template <typename PackageFileType>
            inline static void addTextFileHandler(std::wstring extension, PackageFile* (*handler)(std::u32string))
            {
                static_assert(std::is_final<PackageFileType>::value, "A custom package file type is required to be final.");
                static_assert(std::is_base_of<PackageFile, PackageFileType>::value, "A custom package file is required to be derived from Firework::PackageSystem::PackageFile.");

                PackageManager::textFileHandlers.insert({ std::move(extension), handler });
            }
            inline static void removeTextFileHandler(const std::wstring& extension)
            {
                PackageManager::textFileHandlers.erase(extension);
            }

            static PackageFile* getCorePackageFileByPath(std::wstring filePath);
            
            friend class Firework::Internal::CoreEngine;
        };
    }
}