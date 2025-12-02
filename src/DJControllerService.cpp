#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    auto track_id=track.get_title();
    if(cache.contains(track_id))
    {
        cache.get(track_id);
        return 1;
    } 
    PointerWrapper<AudioTrack> clone=track.clone();
    if(!clone){
        throw std::invalid_argument("This is a NullPointer");
    }
    clone->load();
    clone->analyze_beatgrid();
    bool putted=cache.put(std::move(clone));
    int result=0;
    if (putted)
      result=-1;
    return result;




}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    auto ptr = cache.get(track_title);
    if(ptr){
        return ptr;
    }
    // error log - dor/yagel
    return nullptr;
}
