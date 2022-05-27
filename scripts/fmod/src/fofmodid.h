#ifndef __FOFMOD_ID_H__
#define __FOFMOD_ID_H__


namespace FOFMOD
{
	
	class Identifier
	{
		
		public:
			union
			{
				
				unsigned int data[4];
				struct 
				{
					unsigned int epoch;
					unsigned int generation;
					unsigned int id;
					unsigned int timestamp;
				} mapped;
			};
			
			
			Identifier();
			~Identifier();
	};
}


#endif // __FOFMOD_ID_H__