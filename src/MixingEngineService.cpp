#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // Your implementation here
    std::cout<< "[MixingEngineService] Initialized with 2 empty decks."<<std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout<< "[MixingEngineService] Cleaning updecks..."<<std::endl;    
    for(auto deck : decks){
        if(deck){
            delete deck;
            deck = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout<< "\n=== Loading Track to Deck ==="<<std::endl;    
    auto clone = track.clone();
    if(clone){
        std::cout<< "[ERROR] Track: "<<track.get_title()<<" failed to clone"<<std::endl;
        return -1;
    }
    auto target_deck = 1-active_deck;
    std::cout<< "[Deck Switch] Target deck: "<<target_deck<<""<<std::endl;
    if(decks[target_deck]){
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }
    clone->load();
    clone->analyze_beatgrid();
    if(decks[active_deck] && auto_sync){
        if(!can_mix_tracks(clone->clone())){
            std::cout<< "can’t mix with the cloned track"<<std::endl;
            sync_bpm(clone->clone());
        }
    }
    decks[target_deck] = clone.release();
    std::cout<< "[Load Complete] "<<track.get_title()<<"is now loaded on deck "<<target_deck<<""<<std::endl;
    if(decks[active_deck]){
        std::cout<< "[Unload] Unloading previous deck "<<active_deck<<"("<<track.get_title()<<")"<<std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    active_deck = target_deck;
    std::cout<< "[Active Deck] Switched to deck "<<target_deck<<""<<std::endl;
    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(!decks[active_deck]){
        return false;
    }
    if(!track){
        return false;
    }
    auto active_bpm = decks[active_deck]->get_bpm();
    auto new_bpm = track->get_bpm();
    return std::abs(active_bpm-new_bpm) <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] && track){
        auto original_bpm = track->get_bpm();
        auto sync_bpm = (decks[active_deck]->get_bpm() + original_bpm)/2;
        std::cout<< "[Sync BPM] Syncing BPM from "<<original_bpm<<" to "<<sync_bpm<<""<<std::endl;
    }
}