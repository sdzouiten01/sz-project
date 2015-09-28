//#include "stdafx.h"
#ifndef __PASSER2_H_
#define __PASSER2_H_
#include "tinyxml.h"
#include <string>
#include <hash_map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#include "tbb\spin_mutex.h"

//using namespace tbb;


using namespace std;
using namespace stdext;



	int string_equal(const char * str1, const char * str2)
	{
		if(str1 ==0 && str2==0)
			return 2;
		else if (str1==0 || str2==0)
			return 0;
		string s1(str1);
		string s2(str2);
		if(s1==s2)
			return 1;
		else return 0;
	};


	
	

class key
{
public:
	string product;
	string name;

	void construct(string p, string n)
	{
		product = p;
		name = n;
	}

	bool operator < (const key& k1)const
	{
		if(strcmp(this->product.c_str(), k1.product.c_str())==0)
			return (strcmp(this->name.c_str(), k1.name.c_str())<0);
		else return (strcmp(this->product.c_str(), k1.product.c_str())<0);
	};

	operator   unsigned   long   ()const 
	{ 
		srand(time(0));
		unsigned long r = (unsigned long)rand()/9999999;
		return  (unsigned long)product.c_str()*(unsigned long)name.c_str()*r; 
	} ;

	bool operator==(const key & k1) const 
	{

		return (strcmp(this->name.c_str(), k1.name.c_str())==0)&&(strcmp(this->product.c_str(), k1.product.c_str())==0);
	}




};



class keyt
{
public:
	string product;
	string name;
	string model;
	string column;

	void construct(string p, string n, string m, string c)
	{
		product = p;
		name = n;
		model = m;
		column = c;
	}

	bool operator < (const keyt& k1)const
	{
		if(strcmp(this->product.c_str(), k1.product.c_str())==0)
		{
			if(strcmp(this->model.c_str(),k1.model.c_str())==0)
			{
				if(strcmp(this->name.c_str(), k1.name.c_str())==0)
					return (strcmp(this->column.c_str(), k1.column.c_str())<0);
				else return (strcmp(this->name.c_str(), k1.name.c_str())<0);
			}
			else return (strcmp(this->model.c_str(), k1.model.c_str())<0);
			
		}
		else return (strcmp(this->product.c_str(), k1.product.c_str())<0);
		
	};

	operator   unsigned   long   ()const 
	{ 
		srand(time(0));
		unsigned long r = (unsigned long)rand()/9999999;
		return  ((unsigned long)product.c_str()*(unsigned long)name.c_str()*(unsigned long)model.c_str()*(unsigned long)column.c_str()*r); 
	} ;

	bool operator==(const keyt & k1) const 
	{

		return (strcmp(this->product.c_str(), k1.product.c_str())==0)&&(strcmp(this->name.c_str(), k1.name.c_str())==0)&&(strcmp(this->model.c_str(), k1.model.c_str())==0)&&(strcmp(this->column.c_str(), k1.column.c_str())==0);
	}




};





/*

struct compare_key{
        bool operator()(key k1, key k2) const{
			return (k1.name==k2.name)&&(k1.second_key==k2.second_key);
        }
	};

*/

class content
{
public:
	string type;
	string source;
	string value;
	
};


class table
{
public:
	hash_map<key, content> * hmap1;
	hash_map<keyt, content> * hmap2;

public:
	table()
	{
		hmap1 = new hash_map<key, content>();
		hmap2 = new hash_map<keyt, content>();
	};

	~table()
	{
		delete hmap1;
		delete hmap2;
	}

	int insert(string product, string  name, string  type, string  source, string  value)
	{
		key k;
		k.product = product;
		k.name = name;

		content c;
		c.type =  type;
		c.source = source;
		c.value = value;

		(*hmap1)[k] = c;
		return 0;

	};

	int insert(string * product, string * model, string * name, string * column, string * type, string * source, string * value)
	{
		keyt k;
		k.product = *product;
		k.name = *name;
		k.model =* model;
		k.column = * column;

		content c;
		c.type = * type;
		c.source =* source;
		c.value =* value;

		(*hmap2)[k] = c;

		return 0;
	};


	content search(string * product, string * name)
	{
		content cn;
		cn.value = "no such variable";
		key k;
		k.product = *product;
		k.name = *name;
		const hash_map<key, content>::const_iterator   it = this->hmap1->find(k);
		if(it==hmap1->end())
			return cn;
		return it->second;
	//	else return cn;
	}

	content search(string * product, string * model, string * name, string * column)
	{
		content cn;
		cn.value = "no such variable";
		keyt k;
		k.product = *product;
		k.name = *name;
		k.model = * model;
		k.column = * column;
		const hash_map<keyt, content>::const_iterator   it= this->hmap2->find(k);
		if(it==hmap2->end())
			return cn;
		else 
			return it->second;
	}



	


};


//static spin_mutex passer_mutex;

class Passer 
{
private:
	table * p_table;
//	int type;
//	char * sheet_name;
	string key_discrip;
	string key_data;
//	static Passer * _singletonp;





public:
	Passer(table * ptable)
	{
	//	cout<<"creating mypasser"<<endl;
		p_table = ptable;
	//	type = t;
//		sheet_name = sheetname;
		key_discrip = "Disc";
		key_data = "Var";
	}

/*	static Passer * getInstance (table * ptable)
	{
		spin_mutex::scoped_lock lock(passer_mutex);
		if (NULL == _singletonp)
		{
	//	  std::cout << "creating Passer." << std::endl;
			_singletonp =  new Passer(ptable);
		}
		else
		{
	//        std::cout << "singleton already created!" << std::endl;
		}

		return _singletonp;
	}
	

	static void kill ()
	{
		if (NULL != _singletonp)
		{
			delete _singletonp;
			_singletonp = NULL;
		}
	}*/

	void set(table * ptable)
	{
		p_table = ptable;
//		type = t;
//		sheet_name = sheetname;
	};


	int  pass(char * file)
	{
		TiXmlDocument doc(file);
		bool loadOK = doc.LoadFile();
		if(!loadOK) return 1;
	
		TiXmlHandle doch(&doc);
		TiXmlElement * p_element = doch.FirstChildElement().Element();
		TiXmlHandle rooth(p_element);
		
		TiXmlElement * p_prod_elem = rooth.FirstChild("product").Element();
		while(p_prod_elem && strcmp(p_prod_elem->Value(),"product")==0)
		{
			string prod_name = p_prod_elem->Attribute("product");
			TiXmlHandle prodh(p_prod_elem);
			TiXmlElement * p_default_elem = prodh.FirstChild("default").Element();
			if(p_default_elem)
			{
				TiXmlHandle defaulth(p_default_elem);
				TiXmlElement * p_var_elem = defaulth.FirstChild("variable").Element();
				int count=0;
				while(p_var_elem)
				{
			/*		key k;
					content c;
					k.product = prod_name;
					k.name = p_var_elem->Attribute("name");
					c.source = p_var_elem->Attribute("source");
					c.type = p_var_elem->Attribute("type");
					c.value = p_var_elem->GetText();
					typedef pair(key, content) unit;
					unit u(k,c);
					p_table->hmap1->insert(u);
					p_var_elem = p_var_elem->NextSiblingElement();
					*/

					string name = p_var_elem->Attribute("name");
					string type = p_var_elem->Attribute("type");
					string source = p_var_elem->Attribute("source");
					string value = p_var_elem->GetText();
					p_table->insert(prod_name, name,  type,  source, value);
					p_var_elem = p_var_elem->NextSiblingElement();
					//cout<<++count<<endl;
				}
			}
			


			TiXmlElement * p_non_default_elem = prodh.FirstChild("non-default").Element();
			if(p_non_default_elem)
			{
				TiXmlHandle non_defaulth(p_non_default_elem);
				TiXmlElement * p_var_elem = non_defaulth.FirstChild("variable").Element();
				int count=0;
				while(p_var_elem)
				{
			/*		key k;
					content c;
					k.product = prod_name;
					k.name = p_var_elem->Attribute("name");
					c.source = p_var_elem->Attribute("source");
					c.type = p_var_elem->Attribute("type");
					c.value = p_var_elem->GetText();
					typedef pair(key, content) unit;
					unit u(k,c);
					p_table->hmap1->insert(u);
					p_var_elem = p_var_elem->NextSiblingElement();
					*/
		
			
					string name = p_var_elem->Attribute("name");
					content   c = p_table->search(&prod_name, &name);
					//should add condition of validation
					
					string type = c.type;
					string source = c.source;
					string value = p_var_elem->GetText();
					p_table->insert(prod_name, name,  type,  source, value);
					p_var_elem = p_var_elem->NextSiblingElement();
					//cout<<++count<<endl;
				}
			}
			
			TiXmlElement * p_models_elem = prodh.FirstChild("models").Element();
			if(p_models_elem)
			{
				TiXmlHandle modelsh(p_models_elem);
				TiXmlElement * p_model_elem = modelsh.FirstChild("model").Element();
				while(p_model_elem && strcmp(p_model_elem->Value(),"model")==0)
				{
					// code for traite a model
					TiXmlHandle modelh(p_model_elem);
					TiXmlElement * p_var_elem;
					if(strcmp(prod_name.c_str(), "va")==0)
						p_var_elem = modelh.FirstChild("product_index_variable").FirstChild("variable").Element();
					else
						p_var_elem = modelh.FirstChild("variable").Element();
					while(p_var_elem && strcmp(p_var_elem->Value(),"variable")==0)
					{
						//code for traite a variable

						TiXmlHandle varh(p_var_elem);
						TiXmlElement * p_column_elem = varh.FirstChild("column").Element();

						if(p_column_elem)
						{
							//code for the case that there are several columns for each variable
							while(p_column_elem && strcmp(p_column_elem->Value(), "column")==0)
							{
								//code for traite one column
								string product = prod_name;
								string model = p_model_elem->Attribute("name");
								string name = p_var_elem->Attribute("name");
								string column = p_column_elem->Attribute("index");

								string type = "null";
								string source = "null";
								string value = p_column_elem->GetText();

								p_table->insert(&product, &model, &name, &column, &type, &source, &value);


								p_column_elem = p_column_elem->NextSiblingElement();
							}
						}
						else
						{
							//code for the case that there are only one column for each variable
							string product = prod_name;
							string model = p_model_elem->Attribute("name");
							string name = p_var_elem->Attribute("name");
							string column = "null";

							string type = "null";
							string source = "null";
							string value = p_var_elem->GetText();

							p_table->insert(&product, &model, &name, &column, &type, &source, &value);

						}



						p_var_elem = p_var_elem->NextSiblingElement();
					}


					p_model_elem = p_model_elem->NextSiblingElement();
				}
			}



			p_prod_elem = p_prod_elem->NextSiblingElement();

		}
		
/*

		while(p_element && !string_equal(worksheet_name, sheet_name))
		{
			p_element = p_element->NextSiblingElement();
			worksheet_name = p_element->Attribute("ss:Name");
		}
		
		// no such a worksheet with the name sheet_name
		if(!p_element)	return 1;

		
		
	
		TiXmlHandle worksheeth(p_element);
		p_element = worksheeth.FirstChild("Table").Element();
		TiXmlHandle tableh(p_element);
	
		p_element = tableh.FirstChild("Row").Element();

		switch(type){
	case 1:
			while(p_element)
			{
				TiXmlHandle rowh(p_element);
				TiXmlElement * cell = rowh.ChildElement("Cell", 0).FirstChildElement("Data").Element();
			//	const char * temp1 = cell->GetText();
			//	const char * temp2 = cell->Value();
				if(string_equal(cell->GetText(),"Var"))
				{
					//data structure of source sheet :  name, discription, value, datatype
					string * name = new string(rowh.ChildElement("Cell", 1).FirstChildElement("Data").Element()->GetText());
					string * discrip = new string(rowh.ChildElement("Cell", 2).FirstChildElement("Data").Element()->GetText());
					string * value = new string(rowh.ChildElement("Cell", 3).FirstChildElement("Data").Element()->GetText());
					string * dt = new string(rowh.ChildElement("Cell", 4).FirstChildElement("Data").Element()->GetText());

					p_table->insert(type, name, discrip, value, dt);
				}
				
				else if(!string_equal(cell->GetText(),"Disc"))		return 1;
				p_element = p_element->NextSiblingElement();
			}
			break;
	case 2:
		while(p_element)
			{
				TiXmlHandle rowh(p_element);
				TiXmlElement * cell = rowh.ChildElement("Cell", 0).FirstChildElement("Data").Element();
			//	const char * temp1 = cell->GetText();
			//	const char * temp2 = cell->Value();
				if(string_equal(cell->GetText(),"Var"))
				{
					//data structure of source sheet :  name, discription, value, datatype
					string * name = new string(rowh.ChildElement("Cell", 1).FirstChildElement("Data").Element()->GetText());
					string * discrip = new string(rowh.ChildElement("Cell", 2).FirstChildElement("Data").Element()->GetText());
					string * second_key = new string(rowh.ChildElement("Cell",3).FirstChildElement("Data").Element()->GetText());
					string * value = new string(rowh.ChildElement("Cell", 4).FirstChildElement("Data").Element()->GetText());
					string * dt = new string(rowh.ChildElement("Cell", 5).FirstChildElement("Data").Element()->GetText());

					p_table->insert(type, name, discrip, second_key, value, dt);
				}
				
				else if(!string_equal(cell->GetText(),"Disc"))		return 1;
				p_element = p_element->NextSiblingElement();
			}

		}
		*/
		return 0;
	};


};

//Passer * Passer::_singletonp = NULL;


//static spin_mutex chercher_mutex;


class Chercher
{
protected:
	table * mytableP;
	Passer * mypasserP;
	char * prod_code;
	static Chercher * _singletonc;

	Chercher()
	{
	//	cout<<"creating chercher "<<endl;
		mytableP = new table;
	//	mypasserP = Passer::getInstance(mytableP);
		mypasserP = new Passer(mytableP);
		
	}
	~Chercher()
	{
		delete mytableP;
		delete mypasserP;
	}

	
public:



	static Chercher * getInstance ()
	{
	//	spin_mutex::scoped_lock lock(chercher_mutex);
		if (NULL == _singletonc)
		{
		//  std::cout << "creating chercher." << std::endl;
			_singletonc =  new Chercher();
		}
		else
		{
	//        std::cout << "singleton already created!" << std::endl;
		}

		return _singletonc;
	}
	

	static void kill ()
	{
		if (NULL != _singletonc)
		{
			delete _singletonc;
			_singletonc = NULL;
		}
	}
	void set(char * pd, char * file)
	{
		mypasserP->pass(file);
		this->prod_code = pd;
		
	}
	void set(char * file)
	{
		mypasserP->pass(file);
	}

	string get(string var)
	{
		
		string product("va");
		string model("va_experience");
		string colum(this->prod_code);
		string re1;

		string re2 = mytableP->search(&product, &model, &var, &colum).value;
		if(re2 == "no such variable")
			return re1=mytableP->search(&product, & var).value;
		else 
			return re2;
	}
	string get(string var1, string var2)
	{
		string product("va");
		string model("va_experience");
		if(var2 == "")
			return mytableP->search(&product, & var1).value;
		else
		{
			string re2 = mytableP->search(&product, &model, &var1, &var2).value;
			if(re2 == "no such variable")
				return mytableP->search(&product, & var1).value;
			else 
				return re2;
		}

		
	}

	/*
	string get(string var)
	{
		string product("va");
	//	product = "va";
		return mytableP->search(&product, &var).value;
	}
	*/
};

Chercher* Chercher::_singletonc = NULL;

/*
class ChercherJPN_ADB05 :  Chercher
{
public:
	ChercherJPN_ADB05()
	{
		Chercher();
	}
	string get(string var)
	{
		
		string product("va");
		string model("va_experience");
		string colum("JPN_ADB05");
		string re1;

		string re2 = mytableP->search(&product, &model, &var, &colum).value;
		if(re2 == "no such variable")
			return re1=mytableP->search(&product, & var).value;
		else 
			return re2;
	}
};

class Chercher_ESP_ADB01:Chercher
{
public:
	Chercher_ESP_ADB01()
	{
		Chercher();
	}
	string get(string var)
	{
		
		string product("va");
		string model("va_experience");
		string colum("ESP_ADB01");
		string re1;

		string re2 = mytableP->search(&product, &model, &var, &colum).value;
		if(re2 == "no such variable")
			return re1=mytableP->search(&product, & var).value;
		else 
			return re2;
	}

};

*/
/*
int main()
{
	Chercher chj;
	cout<<chj.get("gmab_av_type");
	getchar();
}


/*
int main()
{

	table mytable;
	passer mypasser(&mytable);
	mypasser.pass();

//	hash_map<const char *, content>::iterator it=0;
//	string s("test");
//	it = mytable.hmap1->find(s.c_str());



	int type=0;
	string product;
	string name;
	string model;
	string column;

	content res;

	while(1)
	{
		cout<<endl;
		cout<<"please set up the variable type that you want to find: "<<endl;
		cin>>type;
		switch(type)
		{
		case 0:
			exit(1);
		case 1:
			cout<<"please type in the name of product that you want: "<<endl;
			cin>>product;
			cout<<"please type in the name of variable that you want: "<<endl;
			cin>>name;
			res=mytable.search(&product, &name);
			cout<<"this is your result: "<<endl;
			cout<<"   "<<product<<"    "<<name<<"    "<<res.value;
			break;
		case 2:
			cout<<"please type in the name of product that you want: "<<endl;
			cin>>product;
			cout<<"please type in the name of model that you want: "<<endl;
			cin>>model;
			
			cout<<"please type in the name of variable that you want: "<<endl;
			cin>>name;
			cout<<"please type in the name of column that you want: "<<endl;
			cin>>column;
			res = mytable.search(&product,&model, &name, &column);
			cout<<"this is your result: "<<endl;
			cout<<product<<"    "<<model<<"    "<<name<<"   "<<column<<"    "<<res.value;
			break;
		}
	

	}



	return 0;
}
*/

#endif