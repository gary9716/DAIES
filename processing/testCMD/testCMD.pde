import java.io.InputStreamReader;

void setup() {

  // what command to run
  String commandToRun = "./installDir/bin/SMILExtract  -C ./config/emobase_live4.conf -nologfile 1";
  // String commandToRun = "ls";
  // String commandToRun = "wc -w sourcefile.extension";
  // String commandToRun = "cp sourcefile.extension destinationfile.extension";
  // String commandToRun = "./yourBashScript.sh";

  File workingDir = new File("/Users/kantei/Documents/soundEmotionRecognition/openEAR-0.1.0");   // where to do it - should be full path
  String returnedValues;                                                                     // value to return any results

  // give us some info:
  println("Running command: " + commandToRun);
  println("Location:        " + workingDir);
  println("---------------------------------------------\n");

  // run the command!
  try {

    // complicated!  basically, we have to load the exec command within Java's Runtime
    // exec asks for 1. command to run, 2. null which essentially tells Processing to 
    // inherit the environment settings from the current setup (I am a bit confused on
    // this so it seems best to leave it), and 3. location to work (full path is best)
    Process p = Runtime.getRuntime().exec(commandToRun, null, workingDir);

    p.waitFor();
    
    // if we have an output, print to screen
    println("got it!");
    // BufferedReader used to get values back from the command
    BufferedReader stdInput = new BufferedReader(new InputStreamReader(p.getErrorStream()));

    // read the output from the command
    while ( (returnedValues = stdInput.readLine ()) != null) {
      println(returnedValues);
    }

  }

  // if there is an error, let us know
  catch (Exception e) {
    println("Error running command!");  
    println(e);
  }

  // when done running command, quit
  println("\n---------------------------------------------");
  println("DONE!");
  exit();
}
