#pragma once
#include <string>
#include <thread>
#include <vector>
namespace LTE
{
    struct note
    {
        note(int chord, double period)
            :chord(chord), period(period){}

        note(double period)
            :period(period){}

        int chord = 0;
        double period;
    };

    /**
        @brief base sequencer class
        this class can be inherited in order to crate a set of musical instraments that each implement is on chords scale, 
        rate and sequence but all oprate together in oreder to crate a programmable band to create in game sond track.
    */
    class sequencer
    {
        protected:

            double bpm;
            int beats, subBeats;
            int totalBeats;
            double BeatTime;
            bool songOn = false;
            std::string sequence;
            std::vector<note> notes; 
            std::thread *songThread;

        public:
            sequencer( double bpm, int beats, int subBeats );

            /**
                @brief PlayBeat -  called every beat or bpm time a minut noteOn/Off the instraments here.
                @param currentTime in sound engine, that the beat happens in.
                @param currentBeat in the sequence.
                @param currentNote that load in the song.
            */
            virtual void PlayBeat(double currentTime, char currentBeat, note currentNote) = 0;

            /**
             *  @brief song - the main function of the sequencer plays the song
             */
            virtual void song(); 

            /**
             *  @brief startSong - start playing the song in a new thrad
             */
            void startSong();

            /**
             *  @brief stopSong - stop the song thrad
             */
            void stopSong();

            
            void setSeqience(std::string sequence){ this->sequence = sequence; };
            void addNote(note toAdd){ notes.push_back(toAdd); };                
    };  
}
