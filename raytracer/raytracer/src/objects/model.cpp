#include "rtpch.h"
#include "objects/model.h"


bool Model::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   HitInfo tempHitInfo;
   bool isHitOccurred = false;
   float closest = maxDist;

   for (const auto& tri : m_Mesh) {
      if (tri.Hit(ray, minDist, maxDist, tempHitInfo) && tempHitInfo.t < closest) {
         isHitOccurred = true;
         closest = tempHitInfo.t;
         hitInfo = tempHitInfo;
         hitInfo.material = m_Material;
      }
   }

   return isHitOccurred;
}

bool Model::Load(const std::string filePath)
{
   m_LoadLineState = LOAD_LINE_STATE::BEGIN_LINE;
   m_LoadTokenState = LOAD_TOKEN_STATE::BEGIN_TOKEN;
   m_LoadTokenType = LOAD_TOKEN_TYPE::LINE_TYPE;

   std::vector<glm::vec3> verts;
   std::vector<glm::i32vec3> faces;
   if (!LoadRaw(filePath, verts, faces)) {
      return false;
   }

   m_Mesh.resize(faces.size());
   for (int i = 0; i < faces.size(); i++) {
      m_Mesh[i] = Triangle(7.0f * verts[faces[i].x - 1], 7.0f *verts[faces[i].y - 1], 7.0f * verts[faces[i].z - 1]);
   }

   return true;
}

bool Model::LoadRaw(const std::string filePath, std::vector<glm::vec3>& verticies, std::vector<glm::i32vec3>& faces)
{
   FILE* file = fopen(filePath.c_str(), "r");
   if (!file) {
#ifdef RT_DEBUG
      wchar_t error[100];
      std::wstring wFilePath(filePath.begin(), filePath.end());
      const wchar_t* fileName = wFilePath.c_str();
      wsprintf(error, L"Model::Load error: couldn't open file^ %s", fileName);
      OutputDebugString(error);
#endif
      return false;
   }
   constexpr int MAX_TOKEN_SIZE = 100;
   char token[MAX_TOKEN_SIZE];
   float fltTokens[3] = { 0.0f, 0.0f, 0.0f };
   int   intTokens[3] = { 0 , 0 , 0 };
   int fltTokenIdx = 0;
   int intTokenIdx = 0;
   int strTokenIdx = 0;
   int nLine = 0;
   int nChar = 0;
   memset(token, 0, MAX_TOKEN_SIZE);

   char ch = fgetc(file);
   while (ch != EOF) {
      LOAD_TOKEN_STATE beforeTokenState = m_LoadTokenState;
      LOAD_TOKEN_TYPE beforeTokenType = m_LoadTokenType;
      if (ProcessChar(ch)) {
         if (m_LoadTokenState == LOAD_TOKEN_STATE::CONTINUE_TOKEN) {
            if (m_LoadTokenType != LOAD_TOKEN_TYPE::LINE_TYPE) {
               token[strTokenIdx++] = ch;
            }
         } else {
            if (m_LoadLineState == LOAD_LINE_STATE::BEGIN_LINE) {
               nChar = 0;
               nLine++;
            }
            if (beforeTokenState != LOAD_TOKEN_STATE::BEGIN_TOKEN && beforeTokenType != LOAD_TOKEN_TYPE::LINE_TYPE) {
               switch (m_LoadTokenType)
               {
                  case Model::LOAD_TOKEN_TYPE::FLOAT:
                  {
                     char * endPtr = &token[strTokenIdx];
                     fltTokens[fltTokenIdx++] = strtof(token, &endPtr);
                     strTokenIdx = 0;
                     memset(token, 0, MAX_TOKEN_SIZE);
                     if (fltTokenIdx == 3) {
                        verticies.emplace_back(fltTokens[0], fltTokens[1], fltTokens[2]);
                        fltTokenIdx = 0;
                     }
                     break;
                  }
                  case Model::LOAD_TOKEN_TYPE::INT:
                     intTokens[intTokenIdx++] = atoi(token);
                     strTokenIdx = 0;
                     memset(token, 0, MAX_TOKEN_SIZE);
                     if (intTokenIdx == 3) {
                        faces.emplace_back(intTokens[0], intTokens[1], intTokens[2]);
                        intTokenIdx = 0;
                     }
                     break;
               }
            }
         }
      } else {
#ifdef RT_DEBUG
         wchar_t error[100];
         std::wstring wFilePath(filePath.begin(), filePath.end());
         const wchar_t* fileName = wFilePath.c_str();
         wsprintf(error, L"Model::Load() error: couldn't parse file: %s. Line %d, character %d", fileName, nLine, nChar);
         OutputDebugString(error);
#endif
         return false;
      }
      nChar++;
      ch = fgetc(file);
   }

   return true;
}

bool Model::ProcessChar(const char ch)
{
   if (m_LoadTokenState == LOAD_TOKEN_STATE::BEGIN_TOKEN) {
      if (m_LoadLineState == LOAD_LINE_STATE::COMMENT) {
         if (ch == '\n') {
            m_LoadLineState = LOAD_LINE_STATE::BEGIN_LINE;
         }
         return true;
      } else if (m_LoadLineState == LOAD_LINE_STATE::BEGIN_LINE) {
         m_LoadTokenType = LOAD_TOKEN_TYPE::LINE_TYPE;
         switch (ch)
         {
            case '#':
            {
               m_LoadLineState = LOAD_LINE_STATE::COMMENT;
               break;
            }
            case 'f':
            {
               m_LoadLineState = LOAD_LINE_STATE::FACE;
               m_LoadTokenType = LOAD_TOKEN_TYPE::INT;
               break;
            }
            case 'l':
            {
               m_LoadLineState = LOAD_LINE_STATE::LINE;
               m_LoadTokenType = LOAD_TOKEN_TYPE::INT;
               break;
            }
            case 'v':
            {
               m_LoadTokenState = LOAD_TOKEN_STATE::CONTINUE_TOKEN;
               m_LoadTokenState = LOAD_TOKEN_STATE::CONTINUE_TOKEN;
               break;
            }
            default:
            {
               return false;
            }
         }
      } else {
         if (!(isdigit(ch) || ch == '-' || ch == ' ')) {
            return false;
         }
         m_LoadTokenState = LOAD_TOKEN_STATE::CONTINUE_TOKEN;
      }
   } else if (m_LoadTokenState == LOAD_TOKEN_STATE::CONTINUE_TOKEN) {
      if (m_LoadLineState == LOAD_LINE_STATE::BEGIN_LINE) {
         switch (ch) {
            case ' ':
            {
               m_LoadLineState = LOAD_LINE_STATE::VERTEX;
               m_LoadTokenType = LOAD_TOKEN_TYPE::FLOAT;
               break;
            }
            case 't':
            {
               m_LoadLineState = LOAD_LINE_STATE::TEXT_COORD;
               m_LoadTokenType = LOAD_TOKEN_TYPE::FLOAT;
               break;
            }
            case 'n':
            {
               m_LoadLineState = LOAD_LINE_STATE::NORMAL;
               m_LoadTokenType = LOAD_TOKEN_TYPE::FLOAT;
               break;
            }
            case 'p':
            {
               m_LoadLineState = LOAD_LINE_STATE::PARAMETER_VERTEX;
               m_LoadTokenType = LOAD_TOKEN_TYPE::FLOAT;
               break;
            }
            default:
            {
               return false;
            }
         }
         m_LoadTokenState = LOAD_TOKEN_STATE::BEGIN_TOKEN;
      } else {
         if (ch == ' ' || ch == '\n') {
            m_LoadTokenState = LOAD_TOKEN_STATE::BEGIN_TOKEN;
            if (ch == '\n') {
               m_LoadLineState = LOAD_LINE_STATE::BEGIN_LINE;
            }
         } else {
            switch (m_LoadLineState) {
               case LOAD_LINE_STATE::VERTEX:
               {
                  if (!(isdigit(ch) || ch == '.' || ch == '-' || ch == 'e')) {
                     return false;
                  }
                  break;
               }
               case LOAD_LINE_STATE::FACE:
               {
                  if (!(isdigit(ch) || ch == '-')) {
                     return false;
                  }
                  break;
               }
               default:
               {
                  return false;
               }
            }
         }
      }
   }

   return true;
}
