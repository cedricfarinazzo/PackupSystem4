import os
from django.shortcuts import render, redirect
from django.contrib.auth import authenticate
from django.contrib.auth.hashers import check_password
from django.contrib.auth import update_session_auth_hash
from django.contrib.auth import login as auth_login
from django.contrib.auth import logout as auth_logout
from django.contrib.auth.models import User
from django.template import loader
from django.conf import settings
from django.core.exceptions import ObjectDoesNotExist
from django.http import HttpResponse, HttpResponseRedirect
import mimetypes
from wsgiref.util import FileWrapper
from .models import *
from .forms import *

# Create your views here.

usage_max = 5000000

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

def normalize_size(size):
    size = float(size)
    if size < 1000:
        return str(size)
    elif size < 1000000:
        size /= 1000
        return ('%.2f'%(size)) + 'K'
    elif size < 1000000000:
        size /= 1000000
        return ('%.2f'%(size)) + 'M'
    elif size < 1000000000000:
        size /= 1000000000
        return ('%.2f'%(size)) + 'G'
    else:
        size /= 1000000000000
        return ('%.2f'%(size)) + 'T'

def backup(request):
    if not request.user.is_authenticated:
        return redirect('/')
    context = {}
    context = {'usage_max': normalize_size(usage_max)}
    if request.method == "POST":
        if 'submit-delete-all' in request.POST:
            Backup.objects.filter(user=request.user).delete()
    try:
        storage = StorageUser.objects.get(user=request.user)
    except ObjectDoesNotExist:
        storage = None
    usage_bytes = 0 if storage is None else storage.usage
    usage_percentage = 100. * usage_bytes / usage_max
    context['usage_bytes'] = normalize_size(usage_bytes)
    context['usage_percentage'] = int(usage_percentage)
    try:
        backups = Backup.objects.filter(user=request.user)
        if backups is None:
            backups = []
    except ObjectDoesNotExist:
        backups = []
    context["backups"] = backups
    return render(request, 'ps4/backup.html', context)

def backup_view(request, id):
    if not request.user.is_authenticated:
        return redirect('/')
    backup = None
    try:
        backup = Backup.objects.filter(user=request.user, id=id).all()[0]
        if backup == None or backup == []:
            raise Exception()
    except:
        return redirect('/backup/')
    if request.method == "POST":
        if 'submit-delete' in request.POST:
            backup.delete()
            return redirect('/backup/')
    context = {'usage_max': normalize_size(usage_max)}
    context['b'] = backup
    try:
        storage = StorageUser.objects.get(user=request.user)
    except ObjectDoesNotExist:
        storage = None
    usage_bytes = 0 if storage is None else storage.usage
    usage_percentage = 100. * usage_bytes / usage_max
    context['usage_bytes'] = normalize_size(usage_bytes)
    context['usage_percentage'] = int(usage_percentage)
    print(context)
    return render(request, 'ps4/backup_view.html', context)

def backup_download(request, id):
    if not request.user.is_authenticated:
        return redirect('/')
    backup = None
    try:
        backup = Backup.objects.filter(user=request.user, id=id).all()[0]
        if backup == None or backup == []:
            raise Exception()
    except:
        return redirect('/backup/')
    file_path = os.path.join(settings.MEDIA_ROOT, backup.backup.url)
    file_wrapper = FileWrapper(open(file_path,'rb'))
    file_mimetype = mimetypes.guess_type(file_path)
    response = HttpResponse(file_wrapper, content_type=file_mimetype )
    response['X-Sendfile'] = file_path
    response['Content-Length'] = os.stat(file_path).st_size
    response['Content-Disposition'] = 'attachment; filename=%s' % (backup.filename)
    return response 

def backup_add(request):
    if not request.user.is_authenticated:
        return redirect('/')
    context = {'usage_max': normalize_size(usage_max)}
    storage = None
    if request.method == 'POST':
        form = BackupForm(request.POST, request.FILES)
        if form.is_valid():
            storage, created = StorageUser.objects.get_or_create(user=request.user)
            if created:
                storage.user = request.user
                storage.usage = 0
            if storage.usage + request.FILES['backup_file'].size  < 5000000:
                storage.usage += request.FILES['backup_file'].size
                storage.save()
                backup = Backup()
                backup.filename = request.FILES['backup_file'].name
                backup.backup = request.FILES['backup_file']
                backup.enc_type = request.POST['enc_type']
                backup.user = request.user
                backup.save()
                context['success'] = "Backup uploaded with success"
            else:
                context['error'] = "Not enough cloud space!"
        else:
            context['error'] = "Please fill out the form"
    if storage is None:
        try:
            storage = StorageUser.objects.get(user=request.user)
        except ObjectDoesNotExist:
            storage = None
    usage_bytes = 0 if storage is None else storage.usage
    usage_percentage = 100. * usage_bytes / usage_max
    context['usage_bytes'] = normalize_size(usage_bytes)
    context['usage_percentage'] = int(usage_percentage)
    return render(request, 'ps4/backup_add.html', context)
