#include "MergedCallbacks.hpp"

MergedCallbacks::MergedCallbacks()
{
}

void MergedCallbacks::registerCallback( CallbackPriority priority, CallbackType callback )
{
	callbacks_[ priority ].push_back( callback );
}

void MergedCallbacks::executeCallbackWithPriority( CallbackPriority priority )
{
	assert( callbacks_.count( priority ) );

	CallbackContainer& callbacks = callbacks_[ priority ];
	for( CallbackContainer::iterator callback_itr = callbacks.begin(); callback_itr != callbacks.end(); ++callback_itr )
	{
		(*callback_itr)();
	}
	callbacks.clear();

	std::cout << "<" << __FUNCTION__ << "> merged the following invocations (with a priority of: ";
    switch(priority)
    {
    	case HIGH:
    		std::cout << "HIGH";
    		break;
    	case NORMAL:
    		std::cout << "NORMAL";
    		break;
    	case LOW:
    		std::cout << "LOW";
    		break;
    	default:
    		std::cout << "UNKNOWN";
    		break;
    }
	 std::cout << "):" << std::endl;

	for ( AccountingType::iterator it = accounting_.begin() ; it != accounting_.end(); ++it )
		std::cout << (*it).first << " => " << (*it).second << std::endl;
	accounting_.clear();
}
