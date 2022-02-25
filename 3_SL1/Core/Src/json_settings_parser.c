/**
  ******************************************************************************
  * @file    json_settings_parser.c
  * @author  dmib@@ETS
  * @brief   This file contains json PDO SC4 settings parser implementation
  ******************************************************************************
  */

#include "stdlib.h"
#include "string.h"
#include "jsonmn.h"
#include "json_settings_parser.h"

/** @addtogroup JSON_SET_PARSER
  * @{
  */

/** @defgroup JSON_LIT JSON string literal defines
  * @brief  JSON PDO SC4 string literal defines
  * @{
  */
#define JSON_GP				"Global parameters"
#define JSON_GP_CID			"cid"
#define JSON_GP_GATEWAY		"gateway"
#define JSON_GP_IP			"ip"
#define JSON_GP_MASK		"mask"
#define JSON_GP_IPDOC		"ip doc"
#define JSON_GP_PORTLOGDOC	"port log doc"
#define JSON_GP_PORTRTPDOC	"port rtp doc"

#define JSON_AB				"activebuttons"
#define JSON_AB_DST			"destination"
#define JSON_AB_DSTIP		"destination_ip"
#define JSON_AB_FUNC        "function"
#define JSON_AB_PRIOR       "priority"
#define JSON_AB_ID          "id"

#define JSON_GRP			"Groups_Calls"
#define JSON_GRP_ID         "id"
#define JSON_GRP_NAME       "name"
#define JSON_GRP_STLIST		"listStations"
#define JSON_STLIST_CID		"cid"
#define JSON_STLIST_IP      "ip"
/**
  * @}
  */

/**
  * @}
  */

/**
  * @brief  JSON Parse state structure
  */
jsmn_parser json_parse;
/**
  * @brief  JSON tokens buffer
  */
jsmntok_t json_tokens[JSON_TOCKENS_MAX]  __attribute__((section(".ExtRamData")));

/**
  * @brief  local convert string containing IPv4-address to IPv4 address array
  * @param  [in] str - string containing IP-address
  * @param  [in] len - number of chars in string
  * @param  [out] ip - IPv4 address array (4 bytes)
  * @retval result : -1 - syntax error; 0 - ok
  */
uint8_t str_to_IP( char* str, uint8_t len, uint8_t *ip){
	uint8_t i,j,j_save;

	j = 0; j_save = 0;
	for(i = 0; i < 4; i++){
		while((str[j] != '.') && (j < len)) j++;
		if ((j >= len) && (i < 3)) return -1;
		ip[i] = atoi(&str[j_save]);
		j_save = ++j;	// skip .
	}
	return 0;
}

/**
  * @brief  string with tokens compare
  * @param  [in] json - string with json
  * @param  [in] tok - token structure
  * @param  [in] s - string to compare
  * @retval result : -1 - not equal; 0 - equal
  */
int8_t jsoneq(char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING)
   if (strlen(s) == (tok->end - tok->start))
	 if (strncmp(json + tok->start, s, tok->end - tok->start) == 0)
	   return 0;
  return -1;
}

/**
  * @brief  parse general settings
  * @param  [in] json_ptr - string with json
  * @param  [in] t_idx - start token number
  * @param  [in] t_num - number of all tokens
  * @param  [out] settings - PDO setting structure
  * @retval t_idx - index of next token after general settings
  */
int16_t Parse_JSON_GP(char *json_ptr, uint32_t t_idx, uint32_t t_num, sPDOSettings *settings)
{
	uint8_t tok_len;
	uint16_t end_pos;

	if (json_tokens[t_idx].type != JSMN_OBJECT) return -1;
	end_pos = json_tokens[t_idx].end;
	t_idx++;

	while ((t_idx < t_num) && (json_tokens[t_idx].start < end_pos)) {
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_GP_CID) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GLOBAL;
		tok_len = json_tokens[t_idx].end - json_tokens[t_idx].start;
		if (tok_len > (SET_CID_SIZE-4)) tok_len = SET_CID_SIZE-4;
		strcpy((char *)settings->CID,"CID");
		strncpy((char *)settings->CID+3,json_ptr + json_tokens[t_idx].start, tok_len);
		settings->CID[tok_len+3]=0;
	  } else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_GP_IP) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GLOBAL;
		tok_len = json_tokens[t_idx].end - json_tokens[t_idx].start;
		str_to_IP(json_ptr + json_tokens[t_idx].start, tok_len, settings->network.ip);
	  } else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_GP_MASK) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GLOBAL;
		tok_len = json_tokens[t_idx].end - json_tokens[t_idx].start;
		str_to_IP(json_ptr + json_tokens[t_idx].start, tok_len, settings->network.mask);
	  }  else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_GP_GATEWAY) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GLOBAL;
		tok_len = json_tokens[t_idx].end - json_tokens[t_idx].start;
		str_to_IP(json_ptr + json_tokens[t_idx].start, tok_len, settings->network.gateway);
	  } else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_GP_IPDOC) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GLOBAL;
		tok_len = json_tokens[t_idx].end - json_tokens[t_idx].start;
		str_to_IP(json_ptr + json_tokens[t_idx].start, tok_len, settings->network.ipdoc);
	  } else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_GP_PORTLOGDOC) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GLOBAL;
		settings->network.portlogdoc = atoi(json_ptr + json_tokens[t_idx].start);
	  } else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_GP_PORTRTPDOC) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GLOBAL;
		settings->network.portrtpdoc = atoi(json_ptr + json_tokens[t_idx].start);
	  }
	  t_idx++;
	}

	return t_idx;
}

/**
  * @brief  parse abonent/buttons settings
  * @param  [in] json_ptr - string with json
  * @param  [in] t_idx - start token number
  * @param  [in] t_num - number of all tokens
  * @param  [out] settings - PDO setting structure
  * @retval t_idx - index of next token after abonent/buttons settings
  */
int16_t Parse_JSON_AB(char *json_ptr, uint32_t t_idx, uint32_t t_num, sPDOSettings *settings)
{
	uint8_t tok_len;
	uint16_t t_idx_save, end_pos, _id;

	if (json_tokens[t_idx].type != JSMN_OBJECT) return -1;
	end_pos = json_tokens[t_idx].end;
	t_idx++;

	t_idx_save = t_idx;
	// search id
	_id = 0;
	while ((t_idx < t_num) && (json_tokens[t_idx].start < end_pos)) {
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_AB_ID) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_AB;
		_id = atoi(json_ptr + json_tokens[t_idx].start);
	  }
	  t_idx++;
	}

	if ((_id > 0) && (_id <= SET_MAX_BUTTONS))
	  _id--;
	else return JSON_ERR_AB;

	t_idx = t_idx_save;
	while ((t_idx < t_num) && (json_tokens[t_idx].start < end_pos)) {
	   if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_AB_DST) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_AB;
		tok_len = json_tokens[t_idx].end - json_tokens[t_idx].start;
		if (tok_len > (SET_CID_SIZE-1)) tok_len = SET_CID_SIZE-1;
		strncpy((char *)settings->buttons[_id].CID, json_ptr + json_tokens[t_idx].start, tok_len);
		settings->buttons[_id].CID[tok_len] = 0;
	  } else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_AB_FUNC) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_AB;
		settings->buttons[_id].func = atoi(json_ptr + json_tokens[t_idx].start);
	  } else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_AB_PRIOR) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_AB;
		settings->buttons[_id].priority = atoi(json_ptr + json_tokens[t_idx].start);
	  } else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_AB_DSTIP) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_AB;
		tok_len = json_tokens[t_idx].end - json_tokens[t_idx].start;
		str_to_IP(json_ptr + json_tokens[t_idx].start, tok_len, settings->buttons[_id].ip);
	  }

	  t_idx++;
	}
	return t_idx;
}

/**
  * @brief  parse station settings
  * @param  [in] json_ptr - string with json
  * @param  [in] t_idx - start token number
  * @param  [in] t_num - number of all tokens
  * @param  [in] grp_id - group number
  * @param  [in] st_idx - station number
  * @param  [out] settings - PDO setting structure
  * @retval t_idx - index of next token after station settings
  */
int16_t Parse_JSON_ST(char *json_ptr, uint32_t t_idx, uint32_t t_num,
                      uint16_t grp_id, uint16_t st_idx, sPDOSettings *settings)
{
	uint8_t tok_len;
	uint16_t end_pos;

	if (json_tokens[t_idx].type != JSMN_OBJECT) return -1;
	end_pos = json_tokens[t_idx].end;
	t_idx++;

	while ((t_idx < t_num) && (json_tokens[t_idx].start < end_pos)) {
	   if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_STLIST_CID) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GRP;
		tok_len = json_tokens[t_idx].end - json_tokens[t_idx].start;
		strncpy((char *)settings->groups[grp_id].stations[st_idx].CID, json_ptr + json_tokens[t_idx].start, tok_len);
		settings->groups[grp_id].stations[st_idx].CID[tok_len] = 0;
	  } else
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_STLIST_IP) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GRP;
		tok_len = json_tokens[t_idx].end - json_tokens[t_idx].start;
		str_to_IP(json_ptr + json_tokens[t_idx].start, tok_len, settings->groups[grp_id].stations[st_idx].ip);
	  }

	  t_idx++;
	}

	return t_idx;
}

/**
  * @brief  parse groups list settings
  * @param  [in] json_ptr - string with json
  * @param  [in] t_idx - start token number
  * @param  [in] t_num - number of all tokens
  * @param  [out] settings - PDO setting structure
  * @retval t_idx - index of next token after groups list settings
  */
int16_t Parse_JSON_GC(char *json_ptr, uint32_t t_idx, uint32_t t_num, sPDOSettings *settings)
{
	uint8_t arr_size;
	uint16_t j, t_idx_save, end_pos, _id;
	int16_t res;

	if (json_tokens[t_idx].type != JSMN_OBJECT) return -1;
	end_pos = json_tokens[t_idx].end;
	t_idx++;

	t_idx_save = t_idx;
	// search id
	_id = 0;
	while ((t_idx < t_num) && (json_tokens[t_idx].start < end_pos)) {
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_GRP_ID) == 0) {
		t_idx++;
		if (t_idx>=t_num) return JSON_ERR_GRP;
		_id = atoi(json_ptr + json_tokens[t_idx].start);
	  }
	  t_idx++;
	}

	if ((_id > 0) && (_id <= SET_MAX_GROUPS)) _id--;
	else return JSON_ERR_GRP;

	settings->groups[_id].id = _id;

	t_idx = t_idx_save;
	while ((t_idx < t_num) && (json_tokens[t_idx].start < end_pos)) {
	  if (jsoneq(json_ptr, &json_tokens[t_idx], JSON_GRP_STLIST) == 0) {
		t_idx++; if (t_idx >= t_num) return JSON_ERR_GRP;
		if (json_tokens[t_idx].type != JSMN_ARRAY) continue;
		arr_size = json_tokens[t_idx].size;
		t_idx++; if (t_idx >= t_num) return JSON_ERR_GRP;

		for (j = 0; j < arr_size; j++) {
		  res = Parse_JSON_ST(json_ptr, t_idx, t_num, _id, j, settings);
		  if (res > 0) {
		   t_idx = res;
		   continue; // not inc t_idx
		  }
		  else return JSON_ERR_GRP;
		}
	  }

	  t_idx++;
	}

	return t_idx;
}

/**
  * @brief  parse all PDO settings
  * @param  [in] json_ptr - string with json
  * @param  [in] json_len - length of JSON (in bytes)
  * @param  [out] settings - PDO setting structure
  * @retval result - JSON parse result code: JSON_OK - settings parsed, else error code
  */
int8_t Parse_JSON(char *json_ptr, uint32_t json_len, sPDOSettings *settings)
{
	int32_t n_tok, res;
	uint16_t i, j, arr_size;

	jsmn_init(&json_parse);
	n_tok = jsmn_parse(&json_parse, (char*)json_ptr, json_len, &json_tokens[0], JSON_TOCKENS_MAX);
	if (n_tok < 1) return JSON_ERR_PARSE;
	if (json_tokens[0].type != JSMN_OBJECT) return JSON_ERR_PARSE;

	i = 1;
	while (i < n_tok){
	  if (jsoneq(json_ptr, &json_tokens[i], JSON_GP) == 0) {
			res = Parse_JSON_GP(json_ptr, i + 1, n_tok, settings);
			if (res > 0) i = res;
			else return JSON_ERR_GLOBAL;
	  } else
	  if (jsoneq(json_ptr, &json_tokens[i], JSON_AB) == 0) {
		  i++; if (i >= n_tok) return JSON_ERR_AB;
		  if (json_tokens[i].type != JSMN_ARRAY) continue;
		  arr_size = json_tokens[i].size;
		  i++; if (i >= n_tok) return JSON_ERR_AB;

		  for (j = 0; j < arr_size; j++) {
			res = Parse_JSON_AB(json_ptr, i, n_tok, settings);
			if (res > 0) i = res;
			else return JSON_ERR_AB;
		  }
	  } else
	  if (jsoneq(json_ptr, &json_tokens[i], JSON_GRP) == 0) {
		  i++; if (i >= n_tok) return JSON_ERR_GRP;
		  if (json_tokens[i].type != JSMN_ARRAY) continue;
		  arr_size = json_tokens[i].size;
		  i++; if (i >= n_tok) return JSON_ERR_GRP;

		  for (j = 0; j < arr_size; j++) {
			res = Parse_JSON_GC(json_ptr, i, n_tok, settings);
			if (res > 0) i = res;
			else return JSON_ERR_GRP;
		  }
	  }
	  else i++;
	}

	return JSON_OK;
}

