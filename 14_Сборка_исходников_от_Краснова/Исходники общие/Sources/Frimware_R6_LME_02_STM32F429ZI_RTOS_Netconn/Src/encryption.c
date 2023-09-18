/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   encryption.c
 * Author: roman.ganaza
 * 
 * Created on May 16, 2023, 10:59 AM
 */

#include "encryption.h"
#include <string.h>
#include <stdio.h>
#include "global_vars.h"
#include "Base64.h"

uint8_t encode(char *hash, char *str) {
  if (base64_enc_len(strlen(str)) < MAX_LOGIN_PASSWORD_BASE64_LEN) {
    base64_encode(hash, str, strlen(str));// TODO: временно шифруем только в BASE64
    return 1;
  } else {
    return 0;
  }
}

void decode(char *hash, char *str) {
  strncpy(str, hash, MAX_LOGIN_PASSWORD_COUPLE_LEN - 1); 
}
