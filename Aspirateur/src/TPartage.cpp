/*
 * TPartage.cpp
 *
 *  Created on: 2017-10-03
 *      Author: jfoucault
 */

#include "TPartage.hpp"

TPartage partage;

TPartage::TPartage() {
	// TODO Auto-generated constructor stub
	controleOk = 0;
	controleBad = 0;
	poids=0;
	sem=TSemaphore(0);
	go=0;
	go2=0;

}

TPartage::~TPartage() {
	// TODO Auto-generated destructor stub
}

string TPartage::getProgression(){
	return progression;
}
float TPartage::getPoids(){
	return poids;

}
bool TPartage::getGo(){
	return go;
}
bool TPartage::getGo2(){
	return go2;
}
void TPartage::setProgression(string valeur){
	progression = valeur;
}
void TPartage::setPoids(float valeur){
	poids = valeur;
}
void TPartage::setGo(bool valeur){
	go =valeur;
}
void TPartage::setGo2(bool valeur){
	go2 =valeur;
}
uint32_t TPartage::getControleOk(void){
	mutexControle.take();
	uint32_t controleReturn= controleOk;
	mutexControle.release();
	return controleReturn;
}
char * TPartage::getControleBad(void){
	mutexControle.take();
	uint32_t controleReturn= controleBad;
	mutexControle.release();
	sprintf(bufferB,"%i",controleReturn);
	return bufferB;
}
void TPartage::protectGetSet(void){
	mutexGetSet.take();
}
void TPartage::unProtectGetSet(void){
	mutexGetSet.release();
}

void TPartage::crementControleOK(void){
	mutexControle.take();
	controleOk++;
	mutexControle.release();

}
void TPartage::crementControleBad(void){
	mutexControle.take();
	controleBad++;
	mutexControle.release();
}
void TPartage::takeSem(){
	sem.take();
}
void TPartage::releaseSem(){
	sem.release();
}
