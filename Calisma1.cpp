#include <iostream>
#include <string>
#include <stdlib.h> 
#include <time.h>
using namespace std;

int global=0;
int main() {
	setlocale(LC_ALL, "Turkish");
	string ing[]={"related","concarned","knowledge","essential","apply","aspects","policy","Deployment","involved","discuss","execute","incredibly","emerging","available","existing","rapidly","consuming","delivery","interwind","remote","accesend","maintained","independent","innovative","combining","dependability","varies","dramatically","diverse","evolved","perhaps","determining","necessary","e-commerce","interakt","incorporate",
	"transaction","interactive","batches","individual","corresponding","billing","salary","payment","entertaiment","primarily","intended","entertain","affered","distinguishing","situations","separate","interacting","computationally","intensive","collect","environment","send","engine","composed","assembly","baundaires","take-into-account","constarints","batch","conjunction","submitted","vehicle","extensive-verification","validation","recall",
	"nonexistent","relies-on","approach","impractial","advance","produced","bahave","expected","secure","external","efficently","waste-resources","specification","requirements","schdule","existing-resources","notions","management","inside"};
	
	string tr[]={"iliskili","endiseli","bilgi","gerekli","basvurmak","bakis acilari","politika","dagitim","dahil olmus","tartismak","uygulamak","inanilmaz","ortaya cikan","mevcut","var olan","hizla","tuketmek","teslim etmek","ic ice","uzak","erisen","bakimli","bagimsiz","yenilikci","birlestirmek","guvenilirlik","degisir","onemli olcude","turlu","gelismis","belki","belirleme","gerekli","e-ticaret","etkilesmek","birlestirmek",
	"islem","etkilesim","partiler","bireysel","karsilik","faturalama","maas","odeme","eglence","oncelikle","amaclanan","eglendirmek","sunulan","ayýrt edici","durumlar","ayirmak","etkilesimli nesneler","hesaplamali olarak","yogun","toplamak","cevre","gondermek","motor","olusan","toplanti","sinirlar","dahil etmek","kisitlamalar","gurup","baglac","gonderilen","arac","kapsamli dogrulama","dogrulama","hatirlamak",
	"var olmayan","dayanir","yaklasmak","pratik olmayan","ilerlemek","uretilmis","davranmak","beklenen","guvenli","harici","verimli","atik kaynaklari","sartname","gereksinim","takvim","mevcut kaynaklar","kavramlar","yonetmek","iceri"};
	
	cout<<"Not:Türkçe Karakter Kullanmayýnýz..."<<endl;
	cout<<"Çýkmak için -1"<<endl<<endl;
	int n = sizeof(ing) / sizeof(ing[0]);
	//cout<<n;
	string tahmin;
	int n1 = sizeof(tr) / sizeof(tr[0]);
	int j,i;
	string kelime;
	while(true){
		srand (time(NULL));
		int a=rand() % sizeof(ing) / sizeof(string);
		kelime=ing[a];
		cout<<a<<". "<<kelime<<" : ";
		getline(cin,tahmin);
		if(tahmin=="-1"){
			return 0 ;
		}
		for(j=0;j<n;j++){
			if(kelime==ing[j]){
				int tut=j;
			
				for(i=0;i<n1;i++){
						if(tahmin==tr[i]){
						
							if(i==j){
								cout<<"Tebrikler Dogru Cevap"<<endl<<endl;
								break;
							}
							
								
							}
							++global;
						}
						
						if(global!=j){
							cout<<"Yanlis -> "<<tr[j]<<" Olacakti"<<endl<<endl;
						
						}
						global=0;
			     	}
				}
			}
		 
		return 0;
	}


