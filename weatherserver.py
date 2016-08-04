import logging

from flask import Flask, render_template, request, url_for, redirect, Response, abort
from json import dumps

from werkzeug.contrib.fixers import ProxyFix


app = Flask(__name__)


@app.before_first_request
def setup_logging():
    if not app.debug:
        # In production mode, add log handler to sys.stderr.
        app.logger.addHandler(logging.StreamHandler())
        app.logger.setLevel(logging.INFO)


@app.route("/", methods=["GET", "POST"])
def landing():
    """
    Receive incoming levels
    """
    if request.method == "POST":
        print request.json
        humidity = request.json.get("humidity")
        temperature = request.json.get("temperature")

        print "Humidity", humidity
        print "Temperature", temperature
    
    return Response(
        response=dumps({"state": "success"}),
        status=200,
        mimetype="application/json",
    )


app.wsgi_app = ProxyFix(app.wsgi_app)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)
