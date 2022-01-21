#INSTALL(FILES Icons/RQExhib.png DESTINATION /reetiPrograms/RApplications/Icons)
function(add_to_install TARGET_NAME)
    INSTALL(TARGETS ${TARGET_NAME}
        DESTINATION ${INSTALL_PATH}
        COMPONENT ${TARGET_NAME})
endfunction ()
#http://apivanliveandlearn.blogspot.com/2015/03/step-by-step-developing-application.html
