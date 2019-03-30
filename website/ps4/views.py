from django.shortcuts import render, redirect
from django.contrib.auth import authenticate
from django.contrib.auth import login as auth_login
from django.contrib.auth import logout as auth_logout
from django.contrib.auth.models import User
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
    if request.user.is_authenticated:
        return redirect('/')
    context = {}
    if request.method == 'POST':
        try:
            username = request.POST['username']
            password = request.POST['password']
            user = authenticate(request, username=username, password=password)
            if user is not None:
                auth_login(request, user)
                return redirect('/')
            else:
                context['error'] = 'Invalid username and/or password'
        except:
            context['error'] = 'An error occured. Please try again'
    return render(request, 'ps4/login.html', context)

def register(request):
    if request.user.is_authenticated:
        return redirect('/')
    
    context = {}
    
    if request.method == "POST":
        try:
            lastname = request.POST['name']
            firtname = request.POST['firstname']
            email = request.POST['email']
            username = request.POST['username']
            password = request.POST['password']
            passcomf = request.POST['passcomfirm']
            
            if password != passcomf:
                context['error'] = 'Passwords didn\'t match'
            else:
                u = User.objects.create_user(username, email, password)
                u.last_name = lastname;
                u.first_name = firtname
                u.save()
                auth_login(request, u)
                return redirect('/')
        except:
            context['error'] = 'An error occured. Please try again'
    return render(request, 'ps4/register.html', context)

def account(request):
    if not request.user.is_authenticated:
        return redirect('/')

    context = {}
    return render(request, 'ps4/account.html', context)

def logout(request):
    if request.user.is_authenticated:
        auth_logout(request)
    return redirect('/')

def about(request):

    context = {}
    return render(request, 'ps4/about.html', context)

def terms(request):

    context = {}
    return render(request, 'ps4/terms.html', context)
