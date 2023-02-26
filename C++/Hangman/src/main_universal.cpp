// https://github.com/Hotsotse/Hang-man
#include <iostream>
#include <string>
#include <random>
using namespace std;

int random(int a, int b){
	thread_local mt19937 eng{random_device{}()};
	uniform_int_distribution<int> dist(a, b);
	return dist(eng);
}

string randomWord_countries() {
	string words[] = {
		"Afghanistan",
		"Albania",
		"Algeria",
		"Andorra",
		"Angola",
		"Argentina",
		"Armenia",
		"Aruba",
		"Australia",
		"Austria",
		"Azerbaijan",
		"Bahamas",
		"Bahrain",
		"Bangladesh",
		"Barbados",
		"Belarus",
		"Belgium",
		"Belize",
		"Benin",
		"Bermuda",
		"Bhutan",
		"Bolivia",
		"Bosnia",
		"Botswana",
		"Brazil",
		"Bulgaria",
		"Burundi",
		"Cambodia",
		"Cameroon",
		"Canada",
		"Chad",
		"Chile",
		"China",
		"Colombia",
		"Comoros",
		"Croatia",
		"Cuba",
		"Curacao",
		"Cyprus",
		"Denmark",
		"Djibouti",
		"Dominica",
		"Ecuador",
		"Egypt",
		"Eritrea",
		"Estonia",
		"Ethiopia",
		"Fiji",
		"Finland",
		"France",
		"Gabon",
		"Gambia",
		"Georgia",
		"Germany",
		"Ghana",
		"Gibraltar",
		"Greece",
		"Greenland",
		"Grenada",
		"Guam",
		"Guatemala",
		"Guinea",
		"Guyana",
		"Haiti",
		"Honduras",
		"Hungary",
		"Iceland",
		"India",
		"Indonesia",
		"Iran",
		"Iraq",
		"Ireland",
		"Israel",
		"Italy",
		"Jamaica",
		"Japan",
		"Jordan",
		"Kazakhstan",
		"Kenya",
		"Kiribati",
		"Kosovo",
		"Kuwait",
		"Latvia",
		"Lebanon",
		"Lesotho",
		"Liberia",
		"Libya",
		"Liechtenstein",
		"Lithuania",
		"Luxembourg",
		"Macedonia",
		"Madagascar",
		"Malawi",
		"Malaysia",
		"Maldives",
		"Mali",
		"Malta",
		"Mauritania",
		"Mauritius",
		"Mexico",
		"Moldova",
		"Monaco",
		"Mongolia",
		"Montenegro",
		"Morocco",
		"Mozambique",
		"Myanmar",
		"Namibia",
		"Nauru",
		"Nepal",
		"Netherlands",
		"Nicaragua",
		"Niger",
		"Nigeria",
		"Norway",
		"Oman",
		"Qatar",
		"Pakistan",
		"Palau",
		"Panama",
		"Paraguay",
		"Peru",
		"Philippines",
		"Poland",
		"Portugal",
		"Romania",
		"Russia",
		"Rwanda",
		"Samoa",
		"Senegal",
		"Serbia",
		"Seychelles",
		"Singapore",
		"Slovenia",
		"Somalia",
		"Spain",
		"Grenadines",
		"SudanR",
		"Suriname",
		"Swaziland",
		"Sweden",
		"Switzerland",
		"Tajikistan",
		"Tanzania",
		"Thailand",
		"Togo",
		"Tonga",
		"Tunisia",
		"Turkey",
		"Turkmenistan",
		"Tuvalu",
		"Uganda",
		"Ukraine",
		"UK",
		"US",
		"Uruguay",
		"Uzbekistan",
		"Vanuatu",
		"Venezuela",
		"Vietnam",
		"Yemen",
		"Zambia",
		"Zimbabwe",
		"Asia",
		"Europe",
		"Africa",
		"Antarctica",
		"Australia",
		"America"
	};
	return words[random(0, 170)];
}

string randomWord_chem(){
	string words[] = {
		"Hydrogen",
		"Helium",
		"Lithium",
		"Beryllium",
		"Boron",
		"Carbon",
		"Nitrogen",
		"Oxygen",
		"Fluorine",
		"Neon",
		"Sodium",
		"Magnesium",
		"Aluminium",
		"Silicon",
		"Phosphorus",
		"Sulphur",
		"Chlorine",
		"Argon",
		"Potassium",
		"Calcium",
		"Scandium",
		"Titanium",
		"Vanadium",
		"Chromium",
		"Manganese",
		"Iron",
		"Cobalt",
		"Nickel",
		"Copper",
		"Zinc",
		"Gallium",
		"Germanium",
		"Arsenic",
		"Selenium",
		"Bromine",
		"Krypton",
		"Rubidium",
		"Strontium",
		"Yttrium",
		"Zirconium",
		"Niobium",
		"Molybdenum",
		"Technetium",
		"Ruthenium",
		"Rhodium",
		"Palladium",
		"Silver",
		"Cadmium",
		"Indium",
		"Tin",
		"Antimony",
		"Tellurium",
		"Iodine",
		"Xenon",
		"Cesium",
		"Barium",
		"Lanthanum",
		"Cerium",
		"Praseodymium",
		"Neodymium",
		"Promethium",
		"Samarium",
		"Europium",
		"Gadolinium",
		"Terbium",
		"Dysprosium",
		"Holmium",
		"Erbium",
		"Thulium",
		"Ytterbium",
		"Lutetium",
		"Hafnium",
		"Tantalum",
		"Tungsten",
		"Rhenium",
		"Osmium",
		"Iridium",
		"Platinum",
		"Gold",
		"Mercury",
		"Thallium",
		"Lead",
		"Bismuth",
		"Polonium",
		"Astatine",
		"Radon",
		"Francium",
		"Radium",
		"Actinium",
		"Thorium",
		"Protactinium",
		"Uranium",
		"Neptunium",
		"Plutonium",
		"Americium",
		"Curium",
		"Berkelium",
		"Californium",
		"Einsteinium",
		"Fermium",
		"Mendelevium",
		"Nobelium",
		"Lawrencium",
		"Rutherfordium",
		"Dubnium",
		"Seaborgium",
		"Bohrium",
		"Hassium",
		"Meitnerium",
		"Darmstadtium",
		"Roentgenium",
		"Copernicium",
		"Flerovium",
		"Nihonium",
		"Moscovium",
		"Tennessine",
		"Oganesson",
		"Livermorium"
	};
	return words[random(0, 117)];
}

string randomWord_animal_easy(){
	string words[] = {
		"ant",
		"bat",
		"bear",
		"cat",
		"crow",
		"cow",
		"chick",
		"camel",
		"deer",
		"dog",
		"duck",
		"eagle",
		"fox",
		"frog",
		"fly",
		"goose",
		"goat",
		"hen",
		"horse",
		"kitty",
		"lion",
		"pig",
		"swan",
		"snack"
		"whale",
		"zebra"
	};
	return words[random(0, 25)];
}

string randomWord_animal_difficult(){
	string words[] = {
		"anteater",
		"butterfly",
		"bullfrog",
		"chicken",
		"canary",
		"dolphin",
		"donkey",
		"drangon",
		"drangonfly"
		"elephant",
		"firefly",
		"giraffe",
		"kitten",
		"kingfisher",
		"parrot",
		"kangaroo",
		"monkey",
		"turkey"
	};
	return words[random(0, 17)];
}

string randomWord_things_easy(){
	string words[] = {
		"bus",
		"cap",
		"cup",
		"car",
		"sofa",
		"door",
		"desk",
		"chair",
		"tape",
		"glove",
		"wallet"
	};
	return words[random(0, 10)];
}

string randomWord_things_difficult(){
	string words[] = {
		"furniture",
		"bookshell",
		"window",
		"battery",
		"flashlight",
		"telephone",
		"television",
		"bookmark"
	};
	return words[random(0, 7)];
}

int letterFill(char guess, string secretword, string &guessword){
	//guess-->指向猜的字
	//secretword-->指向原词
	//guessword-->指向填空
	int matches=0;//猜对个数
	int len = secretword.length();
	
	for (int i = 0; i < len; i++)
	{
		// 是否已经在之前的猜测中匹配了这个字母？
		if (guess == guessword[i]) return 0;
		
		// 猜测的字母是在单词中的字母吗？
		if (guess == secretword[i]){
			guessword[i] = guess;
			matches++;
		}
	}
	return matches;
}

int main(){
	int tries = 6;// 目前还有的尝试次数
	int choose;// 选择单词集
	string word;// 准备好贮存单词
	cout << "欢迎来到 Hangman ！\n有以下单词风格可选择：\n1. 动物-简单    2. 元素名    3. 物品-简单\n4. 动物-复杂    5. 地区名    6. 物品-复杂\n请问您选择--->" ;
	cin>>choose;
	if(choose == 1){
		word = randomWord_animal_easy();// 记录单词
	}else if(choose == 2){
		word = randomWord_chem();
		cout<<"注意元素的首字母要大写哦"<<endl;
	}else if(choose == 3){
		word = randomWord_things_easy();
	}else if(choose == 4){
		word = randomWord_animal_difficult();
	}else if(choose == 5){
		word = randomWord_countries();
		cout<<"注意地名的首字母要大写哦"<<endl;
	}else if(choose == 6){
		word = randomWord_things_difficult();
	}else {
		cout<<"嘿，干嘛！\n哼，不跟你玩了！Ｏ(≧口≦)Ｏ";
		return 0;
	}
	string unknown(word.length(), '_');// 生成下划线串模拟填空
	cout << "随机单词已生成！单词的长度是 " << word.length() << " 个字母！" << endl;
	while (tries > 0){
		cout << unknown << endl;
		char letter; // 记录猜测的字母
		cin >> letter;	
		if (letterFill(letter, word, unknown) == 0){
			cout << "哦，错了！没有在单词中发现 " << letter << " 这个字母！" << endl;
			tries--;
		}
		else {
			cout << "猜对了！" << endl;
		}
		
		cout << "猜一个字母，你还有 " << tries << " 次机会。" << endl;
		
		if (word == unknown)
		{
			cout << word << endl;
			cout << "耶！你赢了！";
			return 0;
		}
	}
	cout << endl << "嘤嘤嘤，你输了。你被绞死了。" << endl;
	cout << "这个词是：" << word << endl;
	//cin.ignore();
	
	return 0;
}
