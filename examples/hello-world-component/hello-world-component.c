/*
 * hello-world.c
 *
 *  Created on: Jan 14, 2015
 *      Author: Francisco Acosta
 *      eMail: fco.ja.ac@gmail.com
 */

#include "hello-world-component.h"
#include "AbstractComponent.h"
#include "AbstractTypeDefinition.h"

#include "contiki.h"
#include <stdlib.h>

/*
 * Parameters definitions
 */
static struct etimer timer;
int count = 0;
/*------------------------------------------------------------------------*/
PROCESS(hw_instances, "kev_contiki//hello_world/0.0.1");
PROCESS(hw_component, "HelloWorldComponent");

HelloWorldComponent *new_HelloWorldComponent()
{
	HelloWorldComponent *pObj = NULL;
	AbstractComponent *pAbsComp = NULL;
	AbstractTypeDefinition *pAbsTypDef = newPoly_AbstractComponent();
	/* Allocating memory */

	if(pAbsTypDef != NULL) {
		pObj = malloc(sizeof(HelloWorldComponent));
		if (pObj != NULL) {
			pObj->super = (AbstractComponent*)pAbsTypDef->pDerivedObj;
		} else {
			return NULL;
		}
	}

	pObj->start = HelloWorldComponent_start;
	pObj->stop = HelloWorldComponent_stop;
	pObj->update = HelloWorldComponent_update;

	pObj->delete = delete_HelloWorldComponent;

	return pObj;
}

/*
 * TODO create HelloWorldComponent::AbstractComponent::AbstractTypeDefinition
 */

AbstractTypeDefinition *newPoly_HelloWorldComponent()
{
	AbstractTypeDefinition *pObj = newPoly_AbstractComponent();
	AbstractComponent *pAbsComp = NULL;
	HelloWorldComponent *pHelloWorld = NULL;

	if(pObj!= NULL) {
		pHelloWorld = malloc(sizeof(HelloWorldComponent));
		if(pHelloWorld == NULL) {
			return NULL;
		}
	}

	pAbsComp = (AbstractComponent*)pObj->pDerivedObj;
	pHelloWorld->super = pAbsComp;
	pAbsComp->pDerivedObj = pHelloWorld;

	pHelloWorld->time = 5;

	if ((pHelloWorld->super->super->params = hashmap_new()) == NULL) {
		printf("ERROR: hashmap cannot be created for this component!\n");
		return NULL;
	} else {
		if ((hashmap_put(pHelloWorld->super->super->params, "time", (void**)&(pHelloWorld->time))) != MAP_OK) {
			return NULL;
		} else {
			printf("INFO: value \"time\" added!\n");
		}
	}

	pObj->start = HelloWorldComponentPoly_start;
	pObj->stop = HelloWorldComponentPoly_stop;
	pObj->update = HelloWorldComponentPoly_update;

	pHelloWorld->start = HelloWorldComponent_start;
	pHelloWorld->stop = HelloWorldComponent_stop;
	pHelloWorld->update = HelloWorldComponent_update;
	pHelloWorld->delete = delete_HelloWorldComponent;

	return pObj;
}

void HelloWorldComponentPoly_start(AbstractTypeDefinition * const this)
{
	AbstractComponent *pAbsComp = (AbstractComponent*)this->pDerivedObj;
	if (pAbsComp != NULL) {
		HelloWorldComponent *pHelloWorld = pAbsComp->pDerivedObj;
		if (pHelloWorld != NULL) {
			count = 0;
			pHelloWorld->start(pHelloWorld);
		} else {
			printf("ERROR: component is not well defined!\n");
		}
	} else {
		printf("ERROR: component is not well defined!\n");
	}
}

void HelloWorldComponentPoly_stop(AbstractTypeDefinition * const this)
{
	AbstractComponent *pAbsComp = (AbstractComponent*)this->pDerivedObj;
	if (pAbsComp != NULL) {
		HelloWorldComponent *pHelloWorld = pAbsComp->pDerivedObj;
		if (pHelloWorld != NULL) {
			pHelloWorld->stop(pHelloWorld);
		} else {
			printf("ERROR: component is not well defined!\n");
		}
	} else {
		printf("ERROR: component is not well defined!\n");
	}
}

void HelloWorldComponentPoly_update(AbstractTypeDefinition * const this, int time)
{
	AbstractComponent *pAbsComp = (AbstractComponent*)this->pDerivedObj;
	if (pAbsComp != NULL) {
		HelloWorldComponent *pHelloWorld = pAbsComp->pDerivedObj;
		if (pHelloWorld != NULL) {
			pHelloWorld->update(pHelloWorld, time);
		} else {
			printf("ERROR: component is not well defined!\n");
		}
	} else {
		printf("ERROR: component is not well defined!\n");
	}
}

void delete_HelloWorldComponent(HelloWorldComponent * const this)
{
	if (this != NULL) {
		free(this);
	}
}

void deletePoly_HelloWorldComponent(AbstractTypeDefinition * const this)
{
	if (this != NULL) {
		AbstractComponent *pAbsComp = (AbstractComponent*)this->pDerivedObj;
		HelloWorldComponent *pHelloWorld = (HelloWorldComponent*)this->pDerivedObj;

		pAbsComp->deletePoly(this);
		pHelloWorld->delete(pHelloWorld);
	}
}

void HelloWorldComponent_start(HelloWorldComponent * const this)
{
	int err;
	int *time;

	printf("HelloWorldComponent is starting!\n");
	if(this->super->super->params != NULL) {
		if ((err = hashmap_get(this->super->super->params, "time", (void**)&time)) == MAP_OK) {
			PROCESS_CONTEXT_BEGIN(&hw_component);
			etimer_set(&timer, (*time) * CLOCK_SECOND);
			PROCESS_CONTEXT_END(&hw_component);
			printf("INFO: timer started at %d seconds\n", (*time) * CLOCK_SECOND);
		} else {
			printf("ERROR: cannot start timer!\n");
		}
	} else {
		printf("ERROR: there are no parameters defined!\n");
	}
}

void HelloWorldComponent_stop(HelloWorldComponent * const this)
{
	printf("HelloWorldComponent is stopping!\n");
	etimer_stop(&timer);
}

void HelloWorldComponent_update(HelloWorldComponent * const this, int time)
{
	printf("HelloWorldComponent is updating!\n");
	AbstractComponent *pAbsComp = this->super;
	AbstractTypeDefinition *pAbsTypDef = pAbsComp->super;

	if (pAbsTypDef->params != NULL) {
		int err;
		int *value;
		if ((err = hashmap_get(pAbsTypDef->params, "time", (void**)&value)) == MAP_OK) {
			*value = time;
			count = 0;
		} else {
			printf("ERROR: cannot retrieve parameter %s! error: %d\n", "time", err);
		}
	} else {
		printf("ERROR: there are no parameters for this component!\n");
	}
}

/*---------------------------------------------------------------------------*/
AUTOSTART_PROCESSES(&hw_instances);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hw_instances, ev, data)
{
	process_start(&hw_component, NULL);
	uint32_t *pointerData;

	PROCESS_BEGIN();
	printf("Starting hw_instance process\n");
	while (1) {
		PROCESS_WAIT_EVENT();
		if (ev == PROCESS_EVENT_POLL) {
			pointerData = (uint32_t*)data;
			*pointerData = (uint32_t)newPoly_HelloWorldComponent();
		}
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hw_component, ev, data)
{
	PROCESS_BEGIN();
	while(1) {
		PROCESS_WAIT_EVENT();
		if (ev == PROCESS_EVENT_TIMER) {
			printf("Hello, world! #%d\n", count++);
			etimer_restart(&timer);
		}
	}
	PROCESS_END();
}
