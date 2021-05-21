#include "guitar.h"
namespace LaughTaleEngine
{
    namespace apoSequencerAndRythemManer
    {
        class guitarString: public envelope
        {
            private:
                double StringFreq;
            public:
                guitarString(double freq)
                {
                    attackTime = 0.1;
                    decayTime = 0.5;
                    releaseTime = 0.0001;
                    startAmplitude= 1.0;
                    sustainAmplitude = 0.0;
                    masterVolume = 1.0/7.0;
                    StringFreq = freq;
                }

                virtual double getSampelAmp(double time, int16_t micAmp)
                {
                    if(!isActive(time)) return 0.0;

                    double amp = getEnvelopeAmpMultiplayer(time);

                    amp = ( amp > 0.0001) * amp;
                    
                    amp *=
                        osc::sin({ time - this->trigerOnTime , StringFreq / 8, 0.00005, StringFreq * 8 }) +
                        osc::sin({ time - this->trigerOnTime , StringFreq / 4, 0.0001, StringFreq * 4 }) +
                        osc::sin({ time - this->trigerOnTime , StringFreq / 2, 0.0005, StringFreq * 2 }) +
                        osc::sin({ time - this->trigerOnTime , StringFreq * 1, 0.001,   StringFreq * 1 }) + 
                        osc::sin({ time - this->trigerOnTime , StringFreq * 2, 0.0005, StringFreq / 2 }) +
                        osc::sin({ time - this->trigerOnTime , StringFreq * 4, 0.0001, StringFreq / 4 }) +
                        osc::sin({ time - this->trigerOnTime , StringFreq * 8, 0.00005, StringFreq / 8 }); 
                    return amp * masterVolume;
                }

        };


        guitar::guitar(): sequencer(90, 4, 4)
        {
            strings = new std::vector<guitarString *>;
            guitarString *EString = new guitarString(082.41);
            guitarString *AString = new guitarString(110.00);
            guitarString *DString = new guitarString(146.83);
            guitarString *GString = new guitarString(196.00);
            guitarString *BString = new guitarString(246.94);
            guitarString *eString = new guitarString(329.63);

            strings->push_back(EString);
            strings->push_back(AString);
            strings->push_back(DString);
            strings->push_back(GString);
            strings->push_back(BString);
            strings->push_back(eString);

            for(envelope * string: *strings)
            {
                soundSynthesizer::addEnvelope(string);
            }  
            
            addNote({1.0});
            setSeqience("^_.._..^..^_...^");
            startSong();           
        }

        void guitar::PlayBeat(double currentTime, char currentBeat, note currentNote)
        {
            if(currentBeat == '^')
                strummingUp((chords)currentNote.chord);
            if(currentBeat == '_')
                strummingDown((chords)currentNote.chord);
        }

        void guitar::strummingDown(chords chord )
        {
            for(guitarString * string: *strings)
            {
                soundSynthesizer::noteOn(string->getId());
                soundSynthesizer::noteOff(string->getId());
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }     
        }
    
        void guitar::strummingUp(chords chord )
        {
            for(std::vector<guitarString*>::iterator string = strings->end() - 1; string != strings->begin(); string--)
            {
                soundSynthesizer::noteOn((*string)->getId());
                soundSynthesizer::noteOff((*string)->getId());
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }     
        }
    }
}