import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.*;

Mixer desiredMixer;
Line desiredLine;

void setup() {
  Mixer.Info[] array = AudioSystem.getMixerInfo(); 
  for( Mixer.Info info:array) {
    if( "C-Media USB Headphone Set".compareTo(info.getName()) == -2 ) 
    //|| "Port C-Media USB Headphone Set".compareTo(info.getName()) == -2 )
    {
      println(info.getName());
      desiredMixer = AudioSystem.getMixer(info);
      Line.Info [] lineInfoArrays = desiredMixer.getTargetLineInfo();
      //println(lineInfoArrays.length);
      /*
      for(Line.Info lineInfo:lineInfoArrays) {
        println(lineInfo.toString());
      }
      */
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
}

void draw() {
   
}

void stop() {
  desiredLine.close();
  super.stop(); 
}
