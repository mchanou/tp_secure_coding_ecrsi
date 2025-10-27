#!/usr/bin/env python3
from flask import Flask, render_template, request
import bleach

ALLOWED_TAGS = ['b', 'i', 'em', 'strong', 'a']
ALLOWED_ATTRS = {'a': ['href', 'rel', 'target']}
ALLOWED_PROTOCOLS = ['http', 'https', 'mailto']

app = Flask(__name__)


comments = []


@app.route('/')
def index():
    return render_template('index.html', comments=comments)

@app.route('/comment', methods=['POST'])
def comment():
    comment = request.form.get('comment', '')
    clean = bleach.clean(comment, tags=ALLOWED_TAGS, attributes=ALLOWED_ATTRS, protocols=ALLOWED_PROTOCOLS, strip=True)
    comments.append(clean)
    return render_template('index.html', comments=comments)

#XSS redirection <img src="/" onerror="window.location.href='https://www.google.com'">



if __name__ == '__main__':
    app.run(debug=True)