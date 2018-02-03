const int pin = 11;
const int song_length = 14;
int song_freq[] = {262,262,392,392,440,440,392,
                   349,349,329,329,294,294,262};
int song_beats[] = {1, 1, 1, 1, 1, 1, 2, 
                    1, 1, 1, 1, 1, 1, 2};
int song_tempo = 150;                    

void setup() {
  pinMode(pin, OUTPUT);
}

void loop() {
  for (int i = 0; i < song_length; i++){
    int duration = song_beats[i]*song_tempo;
    if (song_freq[i] == 0){
      delay(duration);
    } else {
      tone(pin, song_freq[i], duration);
      delay(duration);
    }
  }
  delay(song_tempo/10);
  delay(1000);

}
