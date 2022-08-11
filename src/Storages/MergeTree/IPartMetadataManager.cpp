#include "IPartMetadataManager.h"

#include <Disks/IVolume.h>
#include <Storages/MergeTree/IMergeTreeDataPart.h>

namespace DB
{
IPartMetadataManager::IPartMetadataManager(const IMergeTreeDataPart * part_) : part(part_)
{
}

bool IPartMetadataManager::isCompressFromFileName(const String & file_name)
{
    const auto & extension = fs::path(file_name).extension();
    return isCompressFromMrkExtension(extension) || isCompressFromIndexExtension(extension);
}
}
