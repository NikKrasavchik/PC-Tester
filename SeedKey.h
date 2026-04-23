
typedef enum VKeyGenResultEx
{
  KGRE_Ok = 0,
  KGRE_BufferToSmall = 1,
  KGRE_SecurityLevelInvalid = 2,
  KGRE_VariantInvalid = 3,
  KGRE_UnspecifiedError = 4,
  KGRE_BadDll = 5,
  KGRE_FunctionNotFound = 6
} VKeyGenResultEx_enum;

typedef VKeyGenResultEx_enum (GenerateKeyEx_func)(
   const unsigned char* ipSeedArray, unsigned int iSeedArraySize,
   const unsigned int iSecurityLevel, const char* ipVariant,
   unsigned char* iopKeyArray, unsigned int iMaxKeyArraySize, 
   unsigned int* oActualKeyArraySize);
   
typedef VKeyGenResultEx_enum (GenerateKeyExOpt_func)(
   const unsigned char* ipSeedArray, unsigned int iSeedArraySize,
   const unsigned int iSecurityLevel, const char* ipVariant, const char* ipOptions,
   unsigned char* iopKeyArray, unsigned int iMaxKeyArraySize,
   unsigned int* oActualKeyArraySize);

GenerateKeyEx_func GenerateKeyEx;
GenerateKeyExOpt_func GenerateKeyExOpt;
