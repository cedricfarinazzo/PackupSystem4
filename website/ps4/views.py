from django.shortcuts import render
from django.template import loader
from django.http import HttpResponse

# Create your views here.

def index(request):

    
    context = {}
    return render(request, 'ps4/index.html', context)

def report(request):

    context = {}
    return render(request, 'ps4/report.html', context)

def download(request):

    context = {}
    return render(request, 'ps4/download.html', context)

def login(request):
    #if connected
        # redirect to home

    context = {}
    return render(request, 'ps4/login.html', context)

def register(request):
    #if connected
        # redirect to home

    context = {}
    return render(request, 'ps4/register.html', context)

def account(request):
    #if NOT connected
        # redirect to login

    context = {}
    return render(request, 'ps4/account.html', context)

def logout(request):
    #if connected
        #LOGOUT
    # redirect to home
    pass

def about(request):

    context = {}
    return render(request, 'ps4/about.html', context)

def terms(request):

    context = {}
    return render(request, 'ps4/terms.html', context)
