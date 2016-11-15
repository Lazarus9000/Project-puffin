int inc = 100;
int i = 0;

long postTM = 0;
long measureTM = 0;
long currentTM = 0;
long getTimeTM = 0;
float measurement = 0;
float measurements = 0;

//Variables for NIST time
// const char* host = "utcnist2.colorado.edu";
const char* host = "128.138.141.172";
String TimeDate;

char dataz[200];

void setup() {
  // initialize the digital pin as an output.
  pinMode(1, OUTPUT); //LED on Oak

  // register the cloud function - should be able to do this without a disconnect and connect, but seems to be
  //   buggy/intermittent if I don't - may sometimes the device connects to particle before function() is
  //   called and doesn't work properly when connect has already been called?
  Particle.disconnect();
  Particle.connect();

  // Get some data

  // Trigger the integration
  Particle.publish("getTime2", dataz, PRIVATE);
  Particle.subscribe("hook-response/getTime2", myHandler, MY_DEVICES);
  pinMode(A0, INPUT);
}

void myHandler(const char *event, const char *data) {
  // Handle the integration response
  //TimeDate = data;
  TimeDate = String(data);
  int pos = TimeDate.indexOf("timestamp");
  TimeDate = TimeDate.substring(pos+11, pos+21);
  getTimeTM = millis();
  //memcpy( TimeDate, &data[pch+12], 10 );
  //strncopy(TimeDate, data, pch+
  int f = 12;
    measurement = 0;
    measurements = 0;
    char s[6];
    char t[12];

    long unixTime =  TimeDate.toInt(); 
    
    char str[300];
    strcpy(str, "{\"my-name\":\"");
    sprintf(s, "%d", f);
    strcat(str, s);
    strcat(str, "\",\"my-time\":\"");
    strcat(str, String(unixTime).c_str());
    //strcat(str, TimeDate.c_str());
    //sprintf(t,"%d", TimeDate);
    strcat(str, "\"}");

    Particle.publish("gdokv3", str, 60, PRIVATE);
}

// the loop routine runs over and over again forever:
void loop() {
  currentTM = millis();
  if (currentTM - measureTM > 10) {
    measurement += abs(analogRead(A0)-512);
    measurements++;
    measureTM = currentTM;
  }

  if (currentTM - postTM > 5999) {
    postTM = currentTM;
    // Get some data
    String tempMessage = String(abs(analogRead(A0)-512));
    int asdf = 23;
    String concatmsg = "{\"my-name\":\"";
    concatmsg += "34";
    concatmsg += "\"}";
    char _charmsg[sizeof(concatmsg)];

    int f = int(measurement / measurements);
    measurement = 0;
    measurements = 0;
    char s[6];
    char t[12];

    long unixTime =  TimeDate.toInt(); 
    //unixTime *= 1000;
    unixTime -= getTimeTM / 1000;
    unixTime += currentTM / 1000;
    
    char str[300];
    strcpy(str, "{\"my-name\":\"");
    sprintf(s, "%d", f);
    strcat(str, s);
    strcat(str, "\",\"my-time\":\"");
    strcat(str, String(unixTime).c_str());
    //strcat(str, TimeDate.c_str());
    //sprintf(t,"%d", TimeDate);
    strcat(str, "\"}");

    concatmsg.toCharArray(_charmsg, sizeof(_charmsg));
    Particle.publish("gdokv3", str, 60, PRIVATE);
  }

}
