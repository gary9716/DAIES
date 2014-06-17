/**
 * This sketch demonstrates how to use an AudioRecorder to record audio to disk
 * and then immediately play it back by creating a new FilePlayer using the AudioRecordingStream
 * returned by the save method.
 * <p> 
 * To use this sketch you need to have something plugged into the line-in on your computer.<br/>
 * Press 'r' to toggle recording on and off and the press 's' to save to disk.<br/>
 * The recorded file will be placed in the main folder of the sketch.
 * <p>
 * For more information about Minim and additional features, visit http://code.compartmental.net/minim/
 */

import ddf.minim.*;
import ddf.minim.ugens.*;
//import ddf.minim.effects.*;
import ddf.minim.analysis.*;
//import java.util.LinkedList;
import java.util.*;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.*;

Mixer desiredMixer;
javax.sound.sampled.Line desiredLine;

Minim minim;

// for recording
AudioInput in;
AudioRecorder recorder;

// for playing back
AudioOutput out;
//AudioPlayer player = null;
FilePlayer player = null;
//LowPassFS lpf;

int sampleRate= 44100;//sampleRate of 44100
int freqRange = 2700 + 1;
//int freqRange = sampleRate/2 + 1;

FFT fft;
String note;// name of the note
int n;//int value midi note
color c;//color
float hertz;//frequency in hertz
float midi;//float midi note
int noteNumber;//variable for the midi note
float [] max= new float [freqRange];//array that contains the half of the sampleRate size, because FFT only reads the half of the sampleRate frequency. This array will be filled with amplitude values.
float maximum;//the maximum amplitude of the max array
float frequency;//the frequency in hertz
String audioFileName = "recording.wav";
LinkedList<Integer> freqList;
Iterator iterator = null;
int m = 0;
int freeMode = -1;

int []freqBins = new int[7];
//int []freqBound = new int[6];
int numOfBound = 6;
int [] freqBound = {
  84, 168, 252, 336, 794, 1018
};
final int unitBound = 386;

int [][]LEDconfig = {
  {
    0, 0, 1
  }
  , 
  {
    0, 1, 0
  }
  , 
  {
    0, 1, 1
  }
  , 
  {
    1, 0, 0
  }
  , 
  {
    1, 0, 1
  }
  , 
  {
    1, 1, 0
  }
  , 
  {
    1, 1, 1
  }
};

void setup()
{
  size(512, 200, P3D);
  /*
  Mixer.Info[] array = AudioSystem.getMixerInfo(); 
   for( Mixer.Info info:array) {
   if( "C-Media USB Headphone Set".compareTo(info.getName()) == -2 ) 
   //|| "Port C-Media USB Headphone Set".compareTo(info.getName()) == -2 )
   {
   println(info.getName());
   desiredMixer = AudioSystem.getMixer(info);
   javax.sound.sampled.Line.Info [] lineInfoArrays = desiredMixer.getTargetLineInfo();
   if(lineInfoArrays.length > 0) {
   try {
   desiredLine = desiredMixer.getLine(lineInfoArrays[0]);
   if(!desiredLine.isOpen()) {
   
   desiredLine.open();
   
   }
   else {
   println("desired line is not available");
   }
   }
   catch(Exception e) {
   println(e);
   }
   }
   else {
   println("no line is available");
   }
   }
   }
   */


  minim = new Minim(this);

  // get a stereo line-in: sample buffer length of 2048
  // default sample rate is 44100, default bit depth is 16
  in = minim.getLineIn(Minim.STEREO, 2048, sampleRate);

  // create an AudioRecorder that will record from in to the filename specified.
  // the file will be located in the sketch's main folder.
  recorder = minim.createRecorder(in, audioFileName);

  // get an output we can playback the recording on
  out = minim.getLineOut( Minim.STEREO );

  freqList = new LinkedList<Integer>();

  fft = new FFT(in.left.size(), sampleRate);

  textFont(createFont("Arial", 12));
  /*
  for(int i=0;i<7;i++) {
   freqBound[i] = unitBound*(i+1);
   }
   
   for(int i=0;i<8;i++) {
   freqBins[i] = 0;
   }
   */
}

int getLEDconfig(float inputFreq) {
  for (int i=0; i<numOfBound; i++) {
    if (inputFreq < freqBound[i]) {
      return i;
    }
  }
  if (inputFreq >= freqBound[numOfBound-1]) {
    return numOfBound;
  } else {
    return -1;
  }
}

void draw()
{
  boolean isRecording = recorder.isRecording();
  if (isRecording || freeMode == 1) {
    //append frequency into freqList
    findNote();
    if (isRecording) {
      freqList.add(new Integer((int)frequency));
    }
    //println(frequency);
    println(getLEDconfig(frequency));
  }
  /*
  else {
   int sum = 0;
   for(int i=0;i<8;i++) {
   //print(freqBins[i]);
   //print(" ");
   sum+=freqBins[i]; 
   }
   if(sum!=0) {
   for(int i=0;i<8;i++) {
   print(8*freqBins[i]/(float)sum);
   print(" ");
   }
   println("");
   }
   }
   */

  if (player != null && player.isPlaying()) {
    if (iterator != null && iterator.hasNext()) {
      println(((Integer)iterator.next()).intValue());
    } else {
      iterator = freqList.iterator();
    }
  }


  //visualization
  background(0); 
  stroke(255);
  // draw the waveforms
  // the values returned by left.get() and right.get() will be between -1 and 1,
  // so we need to scale them up to see the waveform
  for (int i = 0; i < in.left.size ()-1; i++)
  {
    line(i, 50 + in.left.get(i)*50, i+1, 50 + in.left.get(i+1)*50);
    line(i, 150 + in.right.get(i)*50, i+1, 150 + in.right.get(i+1)*50);
  }

  if ( isRecording )
  {
    text("Now recording...", 5, 15);
  } else
  {
    text("Not recording.", 5, 15);
  }
}

void findNote() {

  fft.forward(in.left);
  //upper bound = sampleRate/2
  for (int f=0; f< freqRange; f++) { //analyses the amplitude of each frequency analysed, between 0 and 22050 hertz
    max[f]=fft.getFreq(float(f)); //each index is correspondent to a frequency and contains the amplitude value
  }
  maximum=max(max);//get the maximum value of the max array in order to find the peak of volume

  for (int i=0; i<max.length; i++) {// read each frequency in order to compare with the peak of volume
    if (max[i] == maximum) {//if the value is equal to the amplitude of the peak, get the index of the array, which corresponds to the frequency
      frequency= i;
    }
  }

  midi= 69+12*(log((frequency-6)/440));// formula that transform frequency to midi numbers
  n= int (midi);//cast to int

  //the octave have 12 tones and semitones. So, if we get a modulo of 12, we get the note names independently of the frequency  
  if (n%12==9)
  {
    note = ("a");
    c = color (255, 0, 0);
  }

  if (n%12==10)
  {
    note = ("a#");
    c = color (255, 0, 80);
  }

  if (n%12==11)
  {
    note = ("b");
    c = color (255, 0, 150);
  }

  if (n%12==0)
  {
    note = ("c");
    c = color (200, 0, 255);
  }

  if (n%12==1)
  {
    note = ("c#");
    c = color (100, 0, 255);
  }

  if (n%12==2)
  {
    note = ("d");
    c = color (0, 0, 255);
  }

  if (n%12==3)
  {
    note = ("d#");
    c = color (0, 50, 255);
  }

  if (n%12==4)
  {
    note = ("e");
    c = color (0, 150, 255);
  }

  if (n%12==5)
  {
    note = ("f");
    c = color (0, 255, 255);
  }

  if (n%12==6)
  {
    note = ("f#");
    c = color (0, 255, 0);
  }

  if (n%12==7)
  {
    note = ("g");
    c = color (255, 255, 0);
  }

  if (n%12==8)
  {
    note = ("g#");
    c = color (255, 150, 0);
  }
}


void keyReleased()
{
  if ( key == 'r' ) 
  {
    // to indicate that you want to start or stop capturing audio data, 
    // you must callstartRecording() and stopRecording() on the AudioRecorder object. 
    // You can start and stop as many times as you like, the audio data will 
    // be appended to the end of to the end of the file. 
    if ( recorder.isRecording() ) 
    {
      recorder.endRecord();
    } else 
    {
      recorder = minim.createRecorder(in, audioFileName);
      freqList = new LinkedList<Integer>();
      recorder.beginRecord();
    }
  }

  if ( key == 's' ) {
    // we've filled the file out buffer, 
    // now write it to a file of the type we specified in setup
    // in the case of buffered recording, 
    // this will appear to freeze the sketch for sometime, if the buffer is large
    // in the case of streamed recording, 
    // it will not freeze as the data is already in the file and all that is being done
    // is closing the file.
    // save returns the recorded audio in an AudioRecordingStream, 
    // which we can then play with a FilePlayer
    if ( player != null )
    {
      player.unpatch( out );
      player.close();
    }
    iterator = freqList.iterator();
    player = new FilePlayer( recorder.save() );
    player.patch( out );
    player.play();
  }

  if ( key == 'a' ) {
    freeMode = freeMode * -1;
  }

}

void stop() {
  // always close Minim audio classes when you are done with them
  if (desiredLine != null) {
    desiredLine.close();
  }
  if (player != null) {
    player.close();
  }
  if (in != null) {
    in.close();
  }
  if (minim != null) {
    minim.stop();
  }
  super.stop();
}

