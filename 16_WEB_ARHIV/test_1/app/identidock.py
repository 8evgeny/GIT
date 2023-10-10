from flask import Flask
app = Flask(__name__)

#@app.route('/')
#@app.route('/index')
#def index():
#    user = { 'nickname': 'Miguel' } # выдуманный пользователь
#    return '''
#<html>
#  <head>
#    <title>Home Page</title>
#  </head>
#  <body>
#    <h1>Hello, ''' + user['nickname'] + '''</h1>
#  </body>
#</html>



@app.route('/')
def hello_world():
    return 'Hello Docker!\n'

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')


