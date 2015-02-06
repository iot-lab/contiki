/*
 * hello-world.h
 *
 *  Created on: Jan 14, 2015
 *      Author: Francisco Acosta
 *      eMail: fco.ja.ac@gmail.com
 */

#ifndef HELLO_WORLD_H_
#define HELLO_WORLD_H_

/*#pragma ComponentType "HelloWorldComponent"*/

typedef struct _HelloWorldComponent HelloWorldComponent;
typedef struct _AbstractComponent AbstractComponent;
typedef struct _AbstractTypeDefinition AbstractTypeDefinition;

typedef void (*fptrAction)(HelloWorldComponent*);
typedef void (*fptrHWUpdate)(HelloWorldComponent*, int);
typedef void (*fptrHWDelete)(HelloWorldComponent*);
typedef void (*fptrHWPolyDel)(AbstractTypeDefinition*);

typedef struct _HelloWorldComponent {
	AbstractComponent *super;
	int time;
	int count;
	fptrAction start;
	fptrAction stop;
	fptrHWUpdate update;
	fptrHWDelete delete;
	fptrHWPolyDel deletePoly;
} HelloWorldComponent;

HelloWorldComponent *new_HelloWorldComponent(void);
AbstractTypeDefinition *newPoly_HelloWorldComponent(void);
void delete_HelloWorldComponent(HelloWorldComponent * const this);
void deletePoly_HelloWorldComponent(AbstractTypeDefinition * const this);
void HelloWorldComponent_start(HelloWorldComponent * const this);
void HelloWorldComponent_stop(HelloWorldComponent * const this);
void HelloWorldComponent_update(HelloWorldComponent * const this, int time);

/*
 * Support functions
 */

void HelloWorldComponentPoly_start(AbstractTypeDefinition * const this);
void HelloWorldComponentPoly_stop(AbstractTypeDefinition * const this);
void HelloWorldComponentPoly_update(AbstractTypeDefinition * const this, int time);

/*
class HelloWorldComponent :public AbstractComponent
{
	public:
	HelloWorldComponent();
	~HelloWorldComponent();

	 void start();
	 void stop();
	 void update();

};
*/

#endif /* HELLO_WORLD_H_ */
