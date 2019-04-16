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
	somme =0;
	sem=TSemaphore(0);
	etat=1;
	aspirateur="STOP";

}

TPartage::~TPartage() {
	// TODO Auto-generated destructor stub
}

string TPartage::getPoid(){

	return poid;
}
string TPartage::getTare(){

	return tare;
}
string TPartage::getUnite(){

	return unite;
}
char * TPartage::getSomme(){

	sprintf(buffer,"%02X",somme);
	return buffer;
}
void TPartage::setPoid(string pTab){

	poid = pTab;
}
void TPartage::setTare(string pTab){

	tare = pTab;
}
void TPartage::setSomme(char pTab){

	somme = pTab;
}
void TPartage::setUnite(string pTab){

	unite = pTab;
}
string TPartage::getMode() const{
	return mode;
}
string TPartage::getMoteurA() const{
	return moteurA;
}
string TPartage::getMoteurB() const{
	return moteurB;
}
string TPartage::getMoteurC() const{
	return moteurC;
}
string TPartage::getRecetteA() const{
	return recetteA;
}
string TPartage::getRecetteB() const{
	return recetteB;
}
string TPartage::getRecetteC() const{
	return recetteC;
}
string TPartage::getRecetteGo() const{
	return recetteGo;
}
string TPartage::getGoAspirateur() const{
	return aspirateur;
}
int TPartage::getEtat() const{
	return etat;
}
void TPartage::setEtat(int etats){
	etat=etats;
}
void TPartage::setGoAspirateur(string go){
	aspirateur=go;
}
void TPartage::setMode(string mod){
	mode=mod;
}
void TPartage::setMoteurA(string mot){
	moteurA=mot;
}
void TPartage::setMoteurB(string mot){
	moteurB=mot;
}
void TPartage::setMoteurC(string mot){
	moteurC=mot;
}
void TPartage::setRecetteA(string r){
	recetteA=r;
}
void TPartage::setRecetteB(string r){
	recetteB=r;
}
void TPartage::setRecetteC(string r){
	recetteC=r;
}
void TPartage::setRecetteGo(string r){
	recetteGo=r;
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
