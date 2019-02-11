from django.shortcuts import render
from django.template import loader
from django.http import HttpResponse

# Create your views here.

def index(request):

    connected = True # TO CHECK
    nav = {'home': 'active'}
    context = {'title': 'Home', 'nav': nav, 'connected': connected}
    return render(request, 'ps4/index.html', context)

def report(request):

    connected = True # TO CHECK
    nav = {'report': 'active'}
    context = {'title': 'Report', 'nav': nav, 'connected': connected}
    return render(request, 'ps4/report.html', context)

def download(request):

    connected = True # TO CHECK
    nav = {'download': 'active'}
    context = {'title': 'Download', 'nav': nav, 'connected': connected}
    return render(request, 'ps4/download.html', context)

def login(request):
    #if connected
        # redirect to home

    connected = False
    nav = {'login': 'active'}
    context = {'title': 'Login', 'nav': nav, 'connected': connected}
    return render(request, 'ps4/login.html', context)

def register(request):
    #if connected
        # redirect to home

    connected = False
    nav = {'register': 'active'}
    context = {'title': 'Register', 'nav': nav, 'connected': connected}
    return render(request, 'ps4/register.html', context)

def account(request):
    #if NOT connected
        # redirect to login

    connected = True
    nav = {'account': 'active'}
    context = {'title': 'Account', 'nav': nav, 'connected': connected}
    return render(request, 'ps4/account.html', context)

def logout(request):
    #if NOT connected
        # redirect to home

    connected = True
    nav = {'logout': 'active'}
    context = {'title': 'Logout', 'nav': nav, 'connected': connected}
    return render(request, 'ps4/logout.html', context) # OR REDIRECT TO HOME

def about(request):

    connected = True # TO CHECK
    nav = {'about': 'active'}
    context = {'title': 'About', 'nav': nav, 'connected': connected}
    return render(request, 'ps4/about.html', context)

def terms(request):

    connected = True # TO CHECK
    nav = {'terms': 'active'}
    context = {'title': 'Terms', 'nav': nav, 'connected': connected}
    return render(request, 'ps4/terms.html', context)
