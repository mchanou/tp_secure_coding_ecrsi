#!/usr/bin/env python3
from flask import Flask, jsonify, request
from extensions import db,jwt
from auth import auth_bp
from users import user_bp

app = Flask(__name__)

app.config.from_prefixed_env()

#initialize exts
db.init_app(app)
jwt.init_app(app)


#register blueprints
app.register_blueprint(auth_bp,url_prefix='/auth')
app.register_blueprint(user_bp, url_prefix='/users' )

@app.route('/api/login', methods=['GET'])
def login():
    return
    
    

@app.route('/api/hello', methods=['GET'])
def hello():
    return jsonify(message="Hello, World!")

if __name__ == '__main__':
    app.run(debug=True)
