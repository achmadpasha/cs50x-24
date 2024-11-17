import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query for balance the user
    cash = db.execute(
        """
        SELECT cash
        FROM users
        WHERE id = ?
        """,
        session.get('user_id')
    )

    # Lookup to user's portfolio
    portfolios = db.execute(
        """
        SELECT * FROM
            (SELECT symbol, SUM(shares) AS shares
            FROM portfolio
            WHERE user_id = ?
            GROUP BY symbol)
        WHERE shares > 0;
        """,
        session.get('user_id')
    )

    cash = cash[0]['cash']
    total = 0 + cash

    for portfolio in portfolios:
        portfolio["price"] = "{:.2f}".format(lookup(portfolio["symbol"])["price"])
        portfolio["total"] = "{:.2f}".format(float(portfolio["price"]) * portfolio["shares"])

        total += float(portfolio["total"])

        portfolio["price"] = usd(float(portfolio["price"]))
        portfolio["total"] = usd(float(portfolio["total"]))

    return render_template("index.html", portfolio=portfolios, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        # Ensure symbol was submitted and quote was exists
        if not symbol:
            return apology("missing symbol", 400)
        elif quote == None:
            return apology("invalid symbol", 400)

        shares = request.form.get("shares")
        # Ensure shares was submitted and not less than or equal to 0
        if not shares:
            return apology("missing shares", 400)
        elif not shares.isdigit() or int(shares) <= 0:
            return apology("invalid shares", 400)

        shares = int(shares)

        user_id = session.get("user_id")
        # Query for user cash
        cash = db.execute(
            """
            SELECT cash
            FROM users
            WHERE id = ?
            """,
            user_id
        )

        # Capture stock price and total
        price = quote["price"]
        total_price = price * shares

        if cash[0]["cash"] < total_price:
            return apology("not enough cash", 400)

        symbol = quote["symbol"]

        # Insert transaction to database
        db.execute(
            """
            INSERT INTO
            portfolio (user_id, activity, symbol, shares, price, total, time)
            VALUES (?, 'buy', ?, ?, ?, ?, CURRENT_TIMESTAMP)
            """,
            user_id, symbol, shares, price, total_price
        )

        # Update cash of user
        db.execute(
            """
            UPDATE users
            SET cash = cash - ?
            WHERE id = ?
            """,
            total_price, user_id
        )

        # Redirect to homepage and give flash message for stock that user bought
        flash("Bought!")
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    portfolios = db.execute(
        """
        SELECT * FROM portfolio
        WHERE user_id = ?
        """, session.get('user_id')
    )

    for portfolio in portfolios:
        portfolio["price"] = "{:.2f}".format(lookup(portfolio["symbol"])["price"])

    return render_template("history.html", portfolio=portfolios)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == ("POST"):
        quote = lookup(request.form.get("symbol"))

        if quote != None:
            quote['price'] = usd(quote['price'])
            return render_template("quoted.html", quote=quote)
        else:
            return apology("invalid symbol", 400)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == ("POST"):

        # If any left blank
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 400)

        password = request.form.get("password")
        if not password:
            return apology("must provide password", 400)

        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("missing confirmation", 400)

        # If password doesn't match
        if confirmation != password:
            return apology("password doesn't match", 400)

        # Insert user's into database
        try:
            db.execute(
                """
                INSERT INTO
                users (username, hash)
                VALUES (?, ?)
                """,
                username, generate_password_hash(password)
            )
        # If the username is already taken
        except ValueError:
            return apology("username already taken", 400)

        # Log user in
        session["users_id"] = db.execute("SELECT id FROM users WHERE username = ?", username)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session.get('user_id')
    portfolio = db.execute(
        """
         SELECT * FROM
            (SELECT symbol, SUM(shares) AS shares
            FROM portfolio
            WHERE user_id = ?
            GROUP BY symbol)
        WHERE shares > 0;
        """, user_id
    )

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("missing symbol", 400)

        shares = request.form.get("shares")
        if not shares:
            return apology("missing shares", 400)
        elif int(shares) <= 0:
            return apology("invalid shares", 400)

        ownShares = next(key["shares"] for key in portfolio if key["symbol"] == symbol)

        if int(shares) > ownShares:
            return apology("too many shares", 400)

        quote = lookup(symbol)

        price = quote["price"]
        total = float(price) * float(shares)

        db.execute(
            """
            INSERT INTO
            portfolio (user_id, activity, symbol, shares, price, total, time)
            VALUES (?, 'sell', ?, -?, ?, ?, CURRENT_TIMESTAMP)
            """,
            user_id, symbol, shares, price, total
        )

        db.execute(
            """
            UPDATE users
            SET cash = cash + ?
            WHERE id = ?
            """, total, user_id
        )

        flash("Sold!")
        return redirect("/")

    else:
        return render_template("sell.html", portfolio=portfolio)


# PERSONAL FEATURES
@app.route("/account")
@login_required
def account():
    """Open My Account Page"""

    username = db.execute(
        """
        SELECT username FROM users
        WHERE id IN (
            SELECT user_id FROM portfolio
            WHERE user_id = ?
        )
        """, session.get('user_id')
    )

    name = username[0]['username']

    return render_template("account.html", name=name)


@app.route("/changeusername", methods=["POST"])
@login_required
def changeusername():
    """Change Username"""

    # Ensure new username was submitted
    new_username = request.form.get("newusername")
    if not new_username:
        return apology("missing input", 400)

    # Ensure current password was submitted
    password = request.form.get("password")
    if not password:
        return apology("missing password", 400)

    user_id = session.get("user_id")

    # Query database for current password hash
    current_password = db.execute(
        """
        SELECT * FROM users WHERE id IN (
            SELECT user_id FROM portfolio
            WHERE user_id = ?
        )
        """, user_id
    )

    # If password wrong return apology
    if not check_password_hash(current_password[0]["hash"], password):
        return apology("wrong password", 400)
    # Else, update new username to database
    else:
        db.execute(
            """
            UPDATE users
            SET username = ?
            WHERE id IN (
                SELECT user_id FROM portfolio
                WHERE user_id = ?
            )
            """, new_username, user_id
        )

    # Give flash message to the user that username has changed
    flash("Username Changed!")
    # Redirect to account page
    return redirect("/account")


@app.route("/changepassword", methods=["POST"])
@login_required
def changepassword():
    """Change user password"""

    current_pass = request.form.get("currentpassword")
    new_password = request.form.get("newpassword")
    confirm_pass = request.form.get("confirmpassword")
    if not current_pass:
        return apology("missing current password", 400)
    elif not new_password:
        return apology("missing new password", 400)
    elif not confirm_pass:
        return apology("missing confirmation", 400)
    elif new_password != confirm_pass:
        return apology("password doesn't match", 400)

    user_id = session.get("user_id")

    # Query database for current password hash
    rows = db.execute(
        """
        SELECT * FROM users WHERE id IN (
            SELECT user_id FROM portfolio
            WHERE user_id = ?
        )
        """, user_id
    )

    # Ensure current password true
    if not check_password_hash(rows[0]["hash"], current_pass):
        return apology("wrong password", 400)
    else:
        db.execute(
            """
            UPDATE users
            SET hash = ?
            WHERE id IN (
                SELECT user_id FROM portfolio
                WHERE user_id = ?
            )
            """,
            generate_password_hash(new_password),
            user_id
        )

    flash("Password Changed!")
    return redirect("/account")


@app.route("/deleteaccount", methods=["POST"])
@login_required
def deleteaccount():
    """Delete user account"""

    password = request.form.get("password")
    if not password:
        return apology("missing password", 400)

    user_id = session.get("user_id")

    # Query database for user
    user = db.execute(
        """
        SELECT * FROM users
        WHERE id IN (
            SELECT user_id FROM portfolio
            WHERE user_id = ?
        )
        """, user_id
    )

    # Ensure current password true
    if not check_password_hash(user[0]["hash"], password):
        return apology("wrong password", 400)
    else:
        # DELETE data user from database
        db.execute(
            """
            DELETE FROM portfolio
            WHERE user_id = ?
            """, user_id
        )
        db.execute(
            """
            DELETE FROM users
            WHERE id = ?
            """, user[0]["id"]
        )

    return redirect("/login")
