
#include "testApp.h"

	//--------------------------------------------------------------
testApp::testApp(){
}

	//--------------------------------------------------------------
void testApp::setup() {
	
	
	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofEnableSmoothing();
	
		//from box2d addon
	bMouseForce = false;
	
		//set up the box2D world
	box2d.init();
	box2d.setGravity(0, 9.8);
	box2d.createFloor();
	box2d.checkBounds(true);
	box2d.setFPS(30.0);
	
		//jgl bits
	updateTopGraphicPositions(); //set them up for the first time
	
	// font name, size, anti-aliased, full character set
	helloFont.loadFont("font/MuseoSans_500.otf", heightOfTopGraphic - TOPGRAPHICTYPEBOTTOMBORDER - TOPGRAPHICTYPETOPBORDER, true, true);
	helloFont.setLineHeight(128.0f);	
	
		//from ofxtween demo
	unsigned delay = 2000;
	unsigned duration = 1000;
	tweenlinear.setParameters(1,easinglinear,ofxTween::easeOut,0,ofGetWidth()-100,duration,delay);
	
    ofBackground(0,0,0);
	ofSetColor(255,255,255);
	ofNoFill();
	
		//from todds simple twitter example
		// regester for twitter events
	ofAddListener(twitter.TWITTER_RESPONSE, this, &testApp::twitterResponse);
	twitter.allocate();
	
	/*
	 You can load tweets via number and page
	 I am getting a error if I try to call 
	 twitter twice like:
	 
	 twitter.getUserTweets("vanderlin", 20, 1);
	 twitter.getUserTweets("vanderlin", 20, 2);
	 
	 you need to wait for twitter to be done.
	 
	 There is also a twitter profile call
	 but im not done with that :)
	 
	 to use:
	 You need ofxThread
	 you need the altered version of ofxXMLSettings
	 I put it in the src/ofxTwitter should be good to go
	 */
		//twitter.getUserTweets("thehellowall", 20, 1);
		//twitter.getUserMentions();
	
	foregroundColour.r = 255;
	foregroundColour.g = 255;
	foregroundColour.b = 255;	//white
	foregroundColour.a = 220;	//bit of alpha for particles
	
	backgroundColour.r = 0;
	backgroundColour.g = 0;
	backgroundColour.b = 0; //black
	
	bDebugDraw = false;
	
		//vector<string> giveMeSome = tokeniseString("@hellowall this is some blue red more less hello");
	
	loadAndParseXMLColourSettings();
	
	currentShapeState = HELLO_CIRCLE; //draw circles to start
	
	timeBetweenParticles = 2.f; //defaults to 2 seconds between particles, goes between, MINTIMEBETWEENPARTICLES and MAXTIMEBETWEENPARTICLES
	timeOfLastNewParticle = 0.f;
	particleSize = 1.f;	//start with no scaling
	addParticle(); //time of last particle will be altered by addParticle
	
	timeBetweenTwitterUpdateAttempts = 30.f;
	timeOfLastAttemptedTwitterUpdate = ofGetElapsedTimef();
	bGetNewResponse = true;	//start by getting a response
	
	bDoShake = false; //if we should shake the screen on the next frame
	numberOfShakesRemaining = 0;	
	
	allWallInstructions.push_back("No one has tweeted The Hello Wall yet, give it a try. Tweet commands like 'orange bigger slower circles' to @thehellowall");
	allWallInstructions.push_back("Welcome to The Hello Wall!");
	allWallInstructions.push_back("Please Tweet commands to @thehellowall");	
	allWallInstructions.push_back("For example: '@thehellowall plum faster faster bigger bigger triangles'");
	allWallInstructions.push_back("For example: '@thehellowall shake purple circles'");
	allWallInstructions.push_back("For example: '@thehellowall turquoise rectangles shake smaller'");
	allWallInstructions.push_back("For example: '@thehellowall slow slower small skyblue rectangles smaller'");
	allWallInstructions.push_back("You can find more instructions at twitter.com/thehellowall, have fun!");
	
	currentInstruction = 0;
	
	startNewScrollNewInstruction();
	
	lastText = "";
	lastScreenName = "joelgethinlewis";
}

	//--------------------------------------------------------------
void testApp::update() {
	float now = ofGetElapsedTimef();
	
	box2d.update();
	
	if(bMouseForce) {
		float strength = 8.0f;
		float damping  = 0.7f;
		float minDis   = 100;
		for(int i=0; i<customParticleCircles.size(); i++) {
			customParticleCircles[i].addAttractionPoint(mouseX, mouseY, strength, minDis);
			customParticleCircles[i].addDamping(damping, damping);
		}
		for(int i=0; i<customParticleSquares.size(); i++) {
			customParticleSquares[i].addAttractionPoint(mouseX, mouseY, strength, minDis);
			customParticleSquares[i].addDamping(damping, damping);
		}
		for(int i=0; i<customParticleRectangles.size(); i++) {
			customParticleRectangles[i].addAttractionPoint(mouseX, mouseY, strength, minDis);
			customParticleRectangles[i].addDamping(damping, damping);
		}
		for(int i=0; i<customParticleTriangles.size(); i++) {
			customParticleTriangles[i].addAttractionPoint(mouseX, mouseY, strength, minDis);
			customParticleTriangles[i].addDamping(damping, damping);
		}
	}
	
	popAndProcessATweet();
	ofBackground(backgroundColour.r, backgroundColour.g, backgroundColour.b);	

	if((now-timeOfLastNewParticle) > timeBetweenParticles)
	{
		addParticle(); // add em every two seconds
	}
	
	if((now-timeOfLastAttemptedTwitterUpdate) > timeBetweenTwitterUpdateAttempts){
		bGetNewResponse = true;
	}
	
	if(bGetNewResponse){
		twitter.getUserMentions();
		bGetNewResponse = false;
		timeOfLastAttemptedTwitterUpdate = now;
	}	
	
	if(bDoShake && (numberOfShakesRemaining > 0)){
		ofPoint direction = ofPoint(ofRandom(-100.f,100.f), ofRandom(-100.f,100.f));
		
		float strength = ofRandom(20.f, 50.f);
		
		for(int i=0; i<customParticleCircles.size(); i++) {
			customParticleCircles[i].addImpulseForce(direction, strength);
		}
		for(int i=0; i<customParticleSquares.size(); i++) {
			customParticleSquares[i].addImpulseForce(direction, strength);
		}
		for(int i=0; i<customParticleRectangles.size(); i++) {
			customParticleRectangles[i].addImpulseForce(direction, strength);
		}
		for(int i=0; i<customParticleTriangles.size(); i++) {
			customParticleTriangles[i].addImpulseForce(direction, strength);
		}
		
		numberOfShakesRemaining--;
		
		if(numberOfShakesRemaining==0){
			bDoShake = false; //do it NUMBEROFSHAKES times then stop

		}
		
	}
	
	updateTopGraphic();
	
	judgeParticles(); //if they should DIIEEEEEEEEE or not (-;
}


	//--------------------------------------------------------------
void testApp::draw() {
	
	ofEnableSmoothing();
	
	ofSetColor(foregroundColour.r, foregroundColour.g, foregroundColour.b);
			
		//draw any circles
	for(int i=0; i<customParticleCircles.size(); i++) {
			//customParticleCircles[i].setColour(foregroundColour);
		customParticleCircles[i].draw();
	}
		//draw any squares
	for(int i=0; i<customParticleSquares.size(); i++) {
			//customParticleSquares[i].setColour(foregroundColour);
		customParticleSquares[i].draw();
	}	
	
		//draw any rectangles
	for(int i=0; i<customParticleRectangles.size(); i++) {
			//customParticleRectangles[i].setColour(foregroundColour);
		customParticleRectangles[i].draw();
	}	

		//draw any triangles
	for(int i=0; i<customParticleTriangles.size(); i++) {
			//customParticleTriangles[i].setColour(foregroundColour);
		customParticleTriangles[i].draw();
	}		
	
	box2d.draw();
	
	drawTopGraphic();
	
	if(bDebugDraw){
		ofSetColor(255, 128, 0);
		
		px = mouseX;
		py = mouseY;
		
		string info = "";
		info += "Press [f] to toggle Mouse Force ["+ofToString(bMouseForce)+"]\n"; 
		info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
		info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
		info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
		ofSetColor(0, 0, 255);
		ofDrawBitmapString(info, 30, 30);
		
		helloFont.drawString("The Hello Wall", 10, 240);
		helloFont.drawString("Debug Mode, press d to switch back to live", 10, 280);
		
			//from ofx tween
		ofRect(20,230,tweenlinear.update(),20);
		
		ofDrawBitmapString("linear",30,245);
		
//			// draw out the tweets
//		for(int i=0; i<tweets.size(); i++) {
//			ofFill();
//			ofSetColor(255, 255, 0);
//			ofRect(100, 20+(i*30), 30+((int)tweets[i].text.length() * 8), 27);
//			ofSetColor(20, 20, 20);
//			ofDrawBitmapString(tweets[i].text, 120, 38+(i*30));	
//		}

		ofSetColor(0xffffff);
		
			// are we loading
		if(twitter.isLoading()) {
			ofSetColor(255, (ofGetFrameNum()%2)*255, 255);
			ofDrawBitmapString("Loading...", 100, 15);	
		}	
	}
}

	//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	int callID = 0;
	
    switch(key){
        case 'z':
			addParticle();
			break;
		case 's':
			bDoShake = true;
			numberOfShakesRemaining = NUMBEROFSHAKES;
			break;
		case 'm':
			bMouseForce = !bMouseForce;
			break;			
		case 'f':
			ofToggleFullscreen();
			break;
		case ' ':
				//callID = twitter.getUserTweets("thehellowall", 3, 1);
			callID = twitter.getUserMentions(); //auto now
			break;
		case 'd':
			bDebugDraw = !bDebugDraw;
			break;
    }	
}

	//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

	//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

	//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	
}

	//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
	startNewScrollNewInstruction();
}

	//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

	//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}

	//--------------------------------------------------------------
void testApp::twitterResponse(TwitterResponseArgs &e) {
	switch (e.type) {
		case TWITTER_TYPE_STATUS:
			cout << "New status Response - why are these occuring?" << endl;
			break;
		case TWITTER_TYPE_USER:
			cout << "New User Response - why are these occuring?" << endl;
			break;
		case TWITTER_TYPE_MENTIONS:
			cout << "New Mention Response!!!!!" << endl;
			for(int i=0; i< e.tweets.size(); i++) {
				cout << "Tweet recieved but not processed: " << e.tweets[i].text.c_str() << endl;
				tweets.push(e.tweets[i]);
			}
			break;			
	}
	
	
}

void testApp::updateTopGraphicPositions(){
		//should use styles?
	ofFill();
	pt_topTopGraphic.set(0.f,0.f,0.f); //leaving it like this for now
	heightOfTopGraphic = ofGetHeight()/6.f;
	pt_bottomTopGraphic.set(ofGetWidth(), heightOfTopGraphic,0.f);
	ofNoFill();
}

void testApp::updateTopGraphic(){
		//update the tweens and bits here?	
	if(tweenlinear.isCompleted()){
			//start again!
		startNewScrollNewInstruction();
	} //tweens and events are working nice
}

void testApp::drawTopGraphic(){
	ofSetColor(foregroundColour.r, foregroundColour.g, foregroundColour.b);
	ofRect(pt_topTopGraphic.x, pt_topTopGraphic.y, pt_bottomTopGraphic.x, pt_bottomTopGraphic.y);
	ofSetColor(backgroundColour.r, backgroundColour.g, backgroundColour.b);
	helloFont.drawString(allWallInstructions[currentInstruction], tweenlinear.update(), pt_topTopGraphic.y+pt_bottomTopGraphic.y-TOPGRAPHICTYPEBOTTOMBORDER); //10.f border on the type, should be a #define	
}

vector<string> testApp::tokeniseString(string inputString, string deliminators){
	vector<string> splitUp;
	
	StringTokenizer tokenizer(inputString, deliminators, StringTokenizer::TOK_TRIM);
	
	for (StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it)
	{
		std::cout << *it << std::endl;
		splitUp.push_back(*it);
	}	
	
	return splitUp;
}

vector<string> testApp::tokeniseTwitterMessage(ofxTweet tweet){
	vector<string> splitUp;
	
	splitUp = tokeniseString(tweet.text, " "); //break it up around the spaces
	
	return splitUp;
}

void testApp::loadAndParseXMLColourSettings(){
	
	
		//adapted from XML settings addon example, advanced section
	if( xmlColourValues.loadFile("colours/colours.xml") ){
		cout << "colours/colours.xml loaded!" << endl;
	}else{
		cerr << "unable to load colours/colours.xml check data/colours/colours.xml folder" << endl;
	}
		
		//lets see how many <colourlist> </colourlist> tags there are in the xml file
	int numColourListTags = xmlColourValues.getNumTags("colourlist");
	
		//if there is at least one <colourlist> tag we can read the list of colours into the vector of allColourValues
	if(numColourListTags > 0){
		
			//we push into the last colourlist tag
			//this temporarirly treats the tag as
			//the document root.
		xmlColourValues.pushTag("colourlist", numColourListTags-1);
		
			//we see how many points we have stored in <PT> tags
		int numColourTags = xmlColourValues.getNumTags("colour");
		
		allColourValues.resize(numColourTags);
		
		int colourCount = 0;
		
		if(numColourTags > 0){
			
				//We then read those x y values into our
				//array - so that we can then draw the points as
				//a line on the screen
			
				//we have only allocated a certan amount of space for our array
				//so we don't want to read more than that amount of points
			int totalToRead = numColourTags;
			
			for(int i = 0; i < totalToRead; i++){
					//the last argument of getValue can be used to specify
					//which tag out of multiple tags you are refering to.
				string colourName = xmlColourValues.getValue("colour:name", "transparent",i);
				int r = xmlColourValues.getValue("colour:red", 0, i);
				int g = xmlColourValues.getValue("colour:green", 0, i);
				int b = xmlColourValues.getValue("colour:blue", 0, i);
				
				ColourNameAndColour newColour;
				newColour.colourName = colourName;
				newColour.colourValue.r = r;
				newColour.colourValue.g = g;
				newColour.colourValue.b = b;
				
				allColourValues.push_back(newColour);
				colourCount++;
			}
		}else {
			cerr << "Error: no Colours found within Colour List found within XML file, check structure of data/colours/colours.xml " << endl;
		}	
		
			//this pops us out of the colourlist tag
			//sets the root back to the xml document
		xmlColourValues.popTag();
		
		cout << "I know the name and value of " << colourCount << " colours." << endl;
	}else {
		cerr << "Error: no Colour List found within XML file, check structure of data/colours/colours.xml " << endl;
	}	
}

void testApp::checkAndRunColourCommand(string const wallCommand)
{
	
	for (int i =0; i < allColourValues.size(); i++) {
		if(wallCommand == allColourValues[i].colourName)
		{
			cout << "Colour command match! Wallcommand: " << wallCommand << " matches " << allColourValues[i].colourName << endl;
			foregroundColour.r = allColourValues[i].colourValue.r;
			foregroundColour.g = allColourValues[i].colourValue.g;
			foregroundColour.b = allColourValues[i].colourValue.b;
			cout << "The RGB value I have just set is " << foregroundColour.r << ", " << foregroundColour.g << ", " << foregroundColour.b << endl;
		}
	} //should break out, but at least this way it's constant time
}

void testApp::checkAndRunWallCommand(string const wallCommand)
{
    if (wallCommand == "shake") {
		cout << "shake recieved, shaking" << endl;
		bDoShake = true;
		numberOfShakesRemaining = NUMBEROFSHAKES;
	}else if ((wallCommand == "bigger") || (wallCommand == "larger") || (wallCommand == "big") ) { //accept bigger, larger and big
		cout << "bigger recieved, scaling up" << endl;
		increaseSizeOfParticles();
		cout << "new scale is " << particleSize << endl;
	}else if ((wallCommand == "smaller") || (wallCommand == "littler") || (wallCommand == "small") ) {	//accept less or slower
		cout << "smaller recieved, scaling down " << endl;
		decreaseSizeOfParticles();
		cout << "new scale is " << particleSize << endl;
	}else if ((wallCommand == "more") || (wallCommand == "faster") ) { //accept more or faster
		cout << "more recieved, more coming" << endl;
		increaseRateOfParticles();
		cout << "new rate is " << timeBetweenParticles << endl;
	}else if ((wallCommand == "less") || (wallCommand == "slower") ) {	//accept less or slower
		cout << "less recieved, less coming " << endl;
		decreaseRateOfParticles();
		cout << "new rate is " << timeBetweenParticles << endl;
	}else if ((wallCommand == "circle") || (wallCommand == "circles")) { //accept circle or circles
		cout << "circle recieved, changing to circles" << endl;
		currentShapeState = HELLO_CIRCLE;
	}else if ((wallCommand == "square") || (wallCommand == "squares")) { //accept square or squares
		cout << "square recieved, changing to squares" << endl;
		currentShapeState = HELLO_SQUARE;
	}else if ((wallCommand == "rectangle") || (wallCommand == "rectangles")) { //accept rectangle or rectangles
		cout << "rectangle recieved, changing to rectangles" << endl;
		currentShapeState = HELLO_RECTANGLE;
	}else if ((wallCommand == "triangle") || (wallCommand == "triangles")) { //accept triangle or triangles
		cout << "rectangle recieved, changing to rectangles" << endl;
		currentShapeState = HELLO_TRIANGLE;
	}else{
		cout << "colour maybe recieved, checking" << wallCommand << endl;
		checkAndRunColourCommand(wallCommand);
	}
	
		//TODO: add a reset command for quick reset to normal params
	
}

void testApp::processTweet(ofxTweet tweet){
		//first make sure the string is all lower case
	int stringIndex = 0;
	char c;
	
	cout << "String before is " << tweet.text << endl;
	
	while (tweet.text[stringIndex]) {
		c=tweet.text[stringIndex];
		tweet.text[stringIndex] = tolower(c);
		stringIndex++;
	}

	cout << "String after is " << tweet.text << endl;
	
	cout << "New tweet is from " << tweet.screen_name << endl;
	
	if((tweet.text != lastText) || (tweet.screen_name != lastScreenName)){
			//then make sure it's different from the previous message //and it's from a different user - this is ok, as twitter won't let you send the same message twice.....
		vector<string> tokenisedTweetMessage = tokeniseTwitterMessage(tweet);
		
		for(int i = 0; i < tokenisedTweetMessage.size(); i++){
			checkAndRunWallCommand(tokenisedTweetMessage[i]);
		}
		
		lastText = tweet.text;
		lastScreenName = tweet.screen_name;
		
		allWallInstructions[0] = "The last person to Tweet The Hello Wall was @"+lastScreenName+". Hello "+lastScreenName+", thanks!";
	}else{
		cout << "Not a new message, ignoring" << endl;
	}
}

void testApp::popAndProcessATweet(){
	if(!tweets.empty()){ //as long as we some to process
		ofxTweet nextToProcess = tweets.front();
		cout << "About to process the tweet: " << nextToProcess.text << endl;
		processTweet(nextToProcess);
		tweets.pop(); //we've processed it so we can get rid of it
	} 
}

float testApp::getStartX(){
	float startX = ofGetWidth()/2.f +  (ofRandomf()*20.f)  ; //ofRandomf gives range -1 to 1
	
	return startX;
}

void testApp::addHelloCircle(){
	
		//from ofxbox2d example
	CustomParticleCircle circle;
	
	float startY = 20.f;
	float startX = getStartX();
	
	float r = getRandomSizeScaled();		// a random radius 40px - 60px TODO:get this from XML?
	circle.setPhysics(r/100.f, 0.53, 0.01);				// mass = m; bounce = bnc; friction = frc; //p.setPhysics(0.4, 0.53, 0.31);	
	circle.setup(box2d.getWorld(), startX, startY, r);
	circle.setColour(foregroundColour);
	circle.setTimeOfBirth(ofGetElapsedTimef());
	customParticleCircles.push_back(circle);
}

void testApp::addHelloSquare(){
		//from ofxbox2d example
	
	CustomParticleSquare square;
	
	float startY = 20.f;
	float startX = getStartX();	
	
	float w = getRandomSizeScaled();
	
	square.setColour(foregroundColour);
	square.setPhysics((w*w)/10000.f, 0.53, 0.1);
	square.setup(box2d.getWorld(), startX, startY, w, w);
	square.setTimeOfBirth(ofGetElapsedTimef());
	customParticleSquares.push_back(square);	
}

void testApp::addHelloRectangle(){
		//from ofxbox2d example
	
	CustomParticleRectangle rectangle;
	
	float startY = 20.f;
	float startX = getStartX();	
	
	float w = getRandomSizeScaled();	
	float h = getRandomSizeScaled();	
	
	rectangle.setColour(foregroundColour);
	rectangle.setPhysics((w*h)/10000.f, 0.53, 0.1);
	rectangle.setup(box2d.getWorld(), startX, startY, w, h);
	rectangle.setTimeOfBirth(ofGetElapsedTimef());
	customParticleRectangles.push_back(rectangle);	
}

void testApp::addHelloTriangle(){
		//from ofxbox2d example
	
	CustomParticleTriangle triangle;
	
	float startY = 20.f;
	float startX = getStartX();	
	
	float w = getRandomSizeScaled();	
	
	triangle.setColour(foregroundColour);
	triangle.addVertex(-w, 0.f);
	triangle.addVertex(w, 0.f);
	triangle.addVertex(0.f, w); //equilateral?
	triangle.setPhysics((w*(w/2))/10000.f, 0.53, 0.1);	
	if(!triangle.validateShape()){
		cerr << "Invalid triangle creation attempted with dimension " << w << endl;
		return;
	}
	triangle.init(box2d.getWorld(), startX, startY);
	triangle.setTimeOfBirth(ofGetElapsedTimef());
	customParticleTriangles.push_back(triangle);	
}

void testApp::addParticle(){
	
	switch(currentShapeState)
	{
		case HELLO_CIRCLE:
		{
			addHelloCircle();
			break;
		}
		case HELLO_SQUARE:
		{
			addHelloSquare();
			break;
		}
		case HELLO_RECTANGLE:
		{
			addHelloRectangle();
			break;
		}	
		case HELLO_TRIANGLE:
		{
			addHelloTriangle();
			break;
		}				
		default:
		{
			addHelloCircle(); //default to adding circles;
		}
	}		
	
	timeOfLastNewParticle = ofGetElapsedTimef(); //probably bad form
}

void testApp::increaseRateOfParticles(){
	if (timeBetweenParticles > MINTIMEBETWEENPARTICLES ) {
		timeBetweenParticles = timeBetweenParticles - 1.f;
	}
}

void testApp::decreaseRateOfParticles(){
	if (timeBetweenParticles < MAXTIMEBETWEENPARTICLES ) {
		timeBetweenParticles = timeBetweenParticles + 1.f;
	}	
}

void testApp::increaseSizeOfParticles(){
	if (particleSize < MAXPARTICLESIZE ) {
		particleSize = particleSize + 0.1f;
	}
}

void testApp::decreaseSizeOfParticles(){
	if (particleSize > MINPARTICLESIZE ) {
		particleSize = particleSize - 0.1f;
	}	
}

float testApp::getRandomSizeScaled(){	
	float newSize = ofRandom(30.f, 100.f);
	
	newSize *= particleSize; //scale it according to current particle size
	
	return newSize;
}

void testApp::startNewScrollNewInstruction(){
	//from ofxtween demo
	unsigned delay = SCROLLMESSAGEDELAY;
	unsigned duration = SCROLLMESSAGEDURATION;
	
	currentInstruction = (int)ofRandom(0.f, allWallInstructions.size()-1.f);
	
	float widthOfNewText = helloFont.stringWidth(allWallInstructions[currentInstruction]);
	tweenlinear.setParameters(1,easinglinear,ofxTween::easeOut,ofGetWidth(),0.f-(widthOfNewText+10.f),duration,delay);
	
	timeOfLastMessageScroll = ofGetElapsedTimeMillis();
}

void testApp::judgeParticles(){
	
	float now = ofGetElapsedTimef();
	
	//kill any circles
	for(int i=0; i < customParticleCircles.size(); ) { //no i increment, have to control it within the loop to make sure all is valid
		if((now - customParticleCircles[i].timeOfBirth) > LIFEOFPARTICLE){
			customParticleCircles[i].destroyShape();
			customParticleCircles.erase(customParticleCircles.begin()+i);
				//so now i is once again valid, unless at end, when it will just quit out, which is fine
		}else {
			i++;//go onto the next
		}

	}

		//kill any squares
	for(int i=0; i < customParticleSquares.size(); ) { //no i increment, have to control it within the loop to make sure all is valid		if((now - customParticleSquares[i].timeOfBirth) > LIFEOFPARTICLE){
		if((now - customParticleSquares[i].timeOfBirth) > LIFEOFPARTICLE){
			customParticleSquares[i].destroyShape();
			customParticleSquares.erase(customParticleSquares.begin()+i);
				//so now i is once again valid, unless at end, when it will just quit out, which is fine
		}else {
			i++;//go onto the next
		}
		
	}
	
		//kill any rectangles
	for(int i=0; i < customParticleRectangles.size(); ) { //no i increment, have to control it within the loop to make sure all is valid
		if((now - customParticleRectangles[i].timeOfBirth) > LIFEOFPARTICLE){
			customParticleRectangles[i].destroyShape();
			customParticleRectangles.erase(customParticleRectangles.begin()+i);
				//so now i is once again valid, unless at end, when it will just quit out, which is fine
		}else {
			i++;//go onto the next
		}
		
	}
	
		//kill any triangles
	for(int i=0; i < customParticleTriangles.size(); ) { //no i increment, have to control it within the loop to make sure all is valid
		if((now - customParticleTriangles[i].timeOfBirth) > LIFEOFPARTICLE){
			customParticleTriangles[i].destroyShape();
			customParticleTriangles.erase(customParticleTriangles.begin()+i);
				//so now i is once again valid, unless at end, when it will just quit out, which is fine
		}else {
			i++;//go onto the next
		}
		
	}
}