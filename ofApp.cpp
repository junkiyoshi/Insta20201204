#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofNoFill();
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	for (int x = -300; x <= 300; x += 150) {

		for (int y = -300; y <= 300; y += 150) {

			for (int z = -300; z <= 300; z += 150) {

				this->setTriangleToMesh(this->face, this->frame, glm::vec3(x, y, z), ofRandom(30, 50), 0, ofRandom(10, 25));
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 0.37);
	ofRotateY(ofGetFrameNum() * 0.72);

	ofSetColor(39);
	this->face.drawFaces();

	ofSetColor(239);
	this->frame.drawWireframe();

	ofSetColor(39);
	ofDrawBox(glm::vec3(), 750);

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setTriangleToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float start_deg, float height){

	int index = face_target.getNumVertices();

	vector<glm::vec3> vertices;
	for (float deg = start_deg; deg < start_deg + 360; deg += 120) {

		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));
	}

	auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	for (auto& vertex : vertices) {

		auto rotation_z = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.z + ofGetFrameNum() * 0.008), 0, 1, PI * -2.5, PI * 2.5), glm::vec3(0, 0, 1));
		auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.y + ofGetFrameNum() * 0.008), 0, 1, PI * -2.5, PI * 2.5), glm::vec3(0, 1, 0));
		auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.x + ofGetFrameNum() * 0.008), 0, 1, PI * -2.5, PI * 2.5), glm::vec3(1, 0, 0));

		vertex = location + glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x;
	}

	face_target.addVertices(vertices);
	frame_target.addVertices(vertices);

	face_target.addIndex(index + 0); face_target.addIndex(index + 2); face_target.addIndex(index + 4);
	face_target.addIndex(index + 1); face_target.addIndex(index + 3); face_target.addIndex(index + 5);

	face_target.addIndex(index + 0); face_target.addIndex(index + 1); face_target.addIndex(index + 3);
	face_target.addIndex(index + 0); face_target.addIndex(index + 3); face_target.addIndex(index + 2);

	face_target.addIndex(index + 2); face_target.addIndex(index + 3); face_target.addIndex(index + 5);
	face_target.addIndex(index + 2); face_target.addIndex(index + 5); face_target.addIndex(index + 4);

	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 1);
	face_target.addIndex(index + 4); face_target.addIndex(index + 1); face_target.addIndex(index + 2);


	frame_target.addIndex(index + 0); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 4);
	frame_target.addIndex(index + 4); frame_target.addIndex(index + 0);

	frame_target.addIndex(index + 1); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 5); frame_target.addIndex(index + 1);

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 1);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 4); frame_target.addIndex(index + 5);
	
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}