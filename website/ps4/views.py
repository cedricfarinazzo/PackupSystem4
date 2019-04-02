from django.shortcuts import render, redirect
from django.contrib.auth import authenticate
from django.contrib.auth.hashers import check_password
from django.contrib.auth import update_session_auth_hash
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
            firstname = request.POST['firstname']
            email = request.POST['email']
            username = request.POST['username']
            password = request.POST['password']
            passcomf = request.POST['passcomfirm']
            
            if password != passcomf:
                context['error'] = 'Passwords didn\'t match'
            elif lastname == '' or firstname == ''  \
                    or email == '' or username == '' or password == '':
                context['error'] = 'Please fill all field!'
            else:
                u = User.objects.create_user(username, email, password)
                u.last_name = lastname;
                u.first_name = firstname
                u.save()
                auth_login(request, u)
                return redirect('/')
        except:
            context['error'] = 'An error occured. Please try again'
    return render(request, 'ps4/register.html', context)

def account(request):
    if not request.user.is_authenticated:
        return redirect('/')
    user = request.user
    context = {}
    
    if request.method == "POST":
        if 'submit-account' in request.POST:
            try:
                lastname = request.POST['name']
                firstname = request.POST['firstname']
                email = request.POST['email']
                username = request.POST['username']
                if lastname == '' or firstname == ''  \
                    or email == '' or username == '':
                    context['error'] = 'Please fill all field!'
                else:
                    user.last_name = lastname
                    user.username = username
                    user.first_name = firstname
                    user.email = email
                    user.save()
            except:
                context['error'] = 'An error occured. Please try again'
        elif 'submit-pass' in request.POST:
            try:
                curpassword = request.POST['oldpass']
                newpass = request.POST['pass']
                passcomfirm = request.POST['passconfirm']
                if newpass != passcomfirm:
                    context['error'] = 'Passwords didn\'t match'
                elif curpassword == '' or newpass == '':
                    context['error'] = 'Please fill all field!'
                elif not check_password(curpassword, user.password):
                    context['error'] = 'Wrong password!'
                else:
                    user.set_password(newpass)
                    user.save()
                    update_session_auth_hash(request, user)
            except:
                context['error'] = 'An error occured. Please try again'
        elif 'submit-delete' in request.POST:
            try:
                passdel = request.POST['passdel']
                if passdel == '':
                    context['error'] = 'Please fill all field!'
                elif not check_password(passdel, user.password):
                    context['error'] = 'Wrong password!'
                else:
                    user.delete()
                    auth_logout(request)
                    return redirect('/')
            except:
                context['error'] = 'An error occured. Please try again'
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
