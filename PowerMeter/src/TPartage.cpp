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
	m1= false;
	m2 =false;
	m3 =false;
	m4 =false;
	m5 =false;
	m6 =false;
	sem=TSemaphore(0);
	lecture=0;

}

TPartage::~TPartage() {
	// TODO Auto-generated destructor stub
}

bool TPartage::getM1(){

	return m1;
}
bool TPartage::getM2(){

	return m2;
}
bool TPartage::getM3(){

	return m3;
}
bool TPartage::getM4(){

	return m4;
}
bool TPartage::getM5(){

	return m5;
}
bool TPartage::getM6(){

	return m6;
}
string TPartage::getVan(){
	return Van;
}
string TPartage::getVbn(){
	return Vbn;

}
string TPartage::getVab(){
	return Vab;

}
string TPartage::getIa(){
	return Ia;

}
string TPartage::getIb(){
	return Ib;

}
string TPartage::getPuissance(){
	return puissance;

}
string TPartage::getFacPuis(){
	return facPuissance;

}
string TPartage::getEnergetique(){

	return energetique;
}
unsigned char TPartage::getLecture()const{
	return lecture;

}
void TPartage::setM1(bool module){

	m1 = module;
}
void TPartage::setM2(bool module){

	m2 = module;
}
void TPartage::setM3(bool module){

	m3 = module;
}
void TPartage::setM4(bool module){

	m4 = module;
}
void TPartage::setM5(bool module){

	m5 = module;
}
void TPartage::setM6(bool module){

	m6 = module;
}

void TPartage::setVan(string valeur){
	Van = valeur;
}
void TPartage::setVbn(string valeur){
	Vbn = valeur;
}
void TPartage::setVab(string valeur){
	Vab = valeur;
}
void TPartage::setIa(string valeur){
	Ia = valeur;
}
void TPartage::setIb(string valeur){
	Ib = valeur;
}
void TPartage::setPuissance(string valeur){
	puissance = valeur;
}
void TPartage::setFactPuissance(string valeur){
	facPuissance = valeur;
}
void TPartage::setEnergetique(string valeur){
	energetique = valeur;
}
void TPartage::setLecture(unsigned char lec){
	lecture = lec;
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
