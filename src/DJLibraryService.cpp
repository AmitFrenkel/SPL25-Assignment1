#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for(auto track : library_tracks){
        if(track.type == "MP3"){
            MP3Track* mp3_track = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm,
            track.extra_param1, track.extra_param2);
            library.push_back(mp3_track);
            std::cout<<"MP3: MP3Track created: "<<mp3_track->get_bitrate()<<" kbps"<<std::endl;
        }
        else{
            WAVTrack* wav_track = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm,
            track.extra_param1, track.extra_param2);
            library.push_back(wav_track);
            std::cout<<"WAV: WAVTrack created: "<<wav_track->get_sample_rate()<<"Hz/"<<wav_track->get_bit_depth()<<"bit"<<std::endl;
        }
    }
    std::cout<<"[INFO] Track library built: "<<library.size()<<" tracks loaded"<<std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    auto track = playlist.find_track(track_title);
    if(!track){
        return nullptr;
    }
    return track;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout<<"[INFO] Loading playlist: "<<playlist_name<<""<<std::endl;
    playlist = Playlist(playlist_name);
    for(size_t i=0; i<track_indices.size(); i++){
        if(track_indices[i]>=(int)library.size()-1 || track_indices[i]<0){
            std::cout<<"[WARNING] Invalid track index: "<<i<<""<<std::endl;
            continue;
        }
        auto clone = library[track_indices[i]]->clone().release();
        if(!clone){
            std::cout<<"[ERROR] track is null"<<i<<""<<std::endl;
            continue;
        }
        clone->load();
        clone->analyze_beatgrid();
        playlist.add_track(clone);
        std::cout<<"Added "<<clone->get_title()<<" to playlist "<<playlist_name<<""<<std::endl;        
        std::cout<<"[INFO] Playlist loaded: "<<playlist.get_name()<<" ("<<playlist.get_track_count()<<" tracks)"<<std::endl;        
    }
    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> result;
    auto tracks = playlist.getTracks();    
    for(auto track : tracks){
        result.push_back(track->get_title());
    }
    return result;
}