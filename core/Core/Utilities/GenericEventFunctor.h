/*
 *  GenericEventFunctor.h
 *  MWorksCore
 *
 *  Created by bkennedy on 9/6/07.
 *  Copyright 2007 MIT. All rights reserved.
 *
 */



#ifndef _GENERIC_EVENT_FUNCTOR_H
#define _GENERIC_EVENT_FUNCTOR_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "Event.h"
#include "Utilities.h"


BEGIN_NAMESPACE_MW

	
	// abstract base class
	class GenericEventFunctor
		{
		protected:
			std::string _callback_id;
		public:
			GenericEventFunctor(const std::string &callback_id) {
				_callback_id = callback_id;
			}
			
			virtual ~GenericEventFunctor() {};
			// two possible functions to call member function. virtual cause derived
			// classes will use a pointer to an object and a pointer to a member function
			// to make the function call
			virtual void operator()(const shared_ptr<Event> &evt) = 0;  // call using operator
			
			std::string callbackID() const {
				return _callback_id;
			}
		};
    
    class BoostEventFunctor : public GenericEventFunctor {
    protected:
        boost::function<void(shared_ptr<Event>)> functor; 
        
    public:
        BoostEventFunctor(const std::string &callbackID, boost::function< void(shared_ptr<Event>) > _functor):
                        GenericEventFunctor(callbackID){
            functor = _functor;
        }
        
        virtual void operator()(const shared_ptr<Event> &evt){
            functor(evt);
        }
        
        virtual void operator()(shared_ptr<Event> evt){
            functor(evt);
        }
    };


END_NAMESPACE_MW


#endif
