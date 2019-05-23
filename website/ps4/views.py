import os, sys, subprocess
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
    #context['backup_ctn'] = backup.get_archive_content()
    return render(request, 'ps4/backup_view.html', context)


def backup_download(request, id):
    if not request.user.is_authenticated:
        return redirect('/')
    backupf = None
    try:
        backupf = BackupFile.objects.filter(id=id).all()[0]
        if backupf == None or backupf == []:
            raise Exception()
    except:
        return redirect('/backup/')
    file_path = os.path.join(settings.MEDIA_ROOT, backupf.backupfile.url)
    file_wrapper = FileWrapper(open(file_path,'rb'))
    file_mimetype = mimetypes.guess_type(file_path)
    response = HttpResponse(file_wrapper, content_type=file_mimetype )
    response['X-Sendfile'] = file_path
    response['Content-Length'] = os.stat(file_path).st_size
    response['Content-Disposition'] = 'attachment; filename=%s' % (backupf.filename)
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
            usage = 0
            files = request.FILES.getlist('backup_file')
            for e in files:
                usage += e.size
            try:
                usage += request.FILES['backup_public_key_file'].size
            except:
                pass
            try:
                usage += request.FILES['backup_private_key_file'].size
            except:
                pass
            try:
                usage += request.FILES['backup_dico_file'].size
            except:
                pass
            if storage.usage + usage < usage_max:
                storage.usage += usage
                storage.save()
                backup = Backup()
                backup.enc_type = request.POST['enc_type']
                backup.comp_type = request.POST['comp_type']
                backup.user = request.user
                backup.pass_backup = ""
                if backup.enc_type in [Backup.ROTN, Backup.VIG, Backup.AES]:
                    try:
                        backup.pass_backup = request.POST['backup_pass']
                    except:
                        pass
                backup.save()
                for e in files:
                    bf = BackupFile()
                    bf.user = request.user
                    bf.backupfile = e
                    bf.filename = e.name
                    bf.backup = backup
                    bf.save()
                if backup.enc_type in [Backup.RSA, Backup.EL]:
                    try:
                        pub = BackupFile()
                        pub.user = request.user
                        pub.backupfile = request.FILES['backup_public_key_file']
                        pub.filename = request.FILES['backup_public_key_file'].name
                        pub.backup = backup
                        pub.file_type = BackupFile.PUBLIC_KEY
                        pub.save()
                    except:
                        pass
                    try:
                        pri = BackupFile()
                        pri.user = request.user
                        pri.backupfile = request.FILES['backup_private_key_file']
                        pri.filename = request.FILES['backup_private_key_file'].name
                        pri.backup = backup
                        pri.file_type = BackupFile.PRIVATE_KEY
                        pri.save()
                    except:
                        pass
                if backup.comp_type == Backup.LZ78:
                    try:
                        dic = BackupFile()
                        dic.user = request.user
                        dic.backupfile = request.FILES['backup_dico_file']
                        dic.filename = request.FILES['backup_dico_file'].name
                        dic.backup = backup
                        dic.file_type = BackupFile.LZ_DICO
                        dic.save()
                    except:
                        pass
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


def backup_view_content(request, id):
    if not request.user.is_authenticated:
        return redirect('/')
    backup = None
    error = False
    html = ""
    try:
        backup = Backup.objects.filter(user=request.user, id=id).all()[0]
        if backup == None or backup == []:
            raise Exception()
    except:
        html += "[<span class=\"red-text\">ERROR</span>]: No backup found"
        error = True
    if not error:
        archive_content = backup.get_archive_content()
        if archive_content is None:
            if backup.enc_type in [Backup.AES, Backup.ROTN, Backup.VIG]:
                if backup.pass_backup == "":
                    html += "[<span class=\"red-text\">ERROR</span>]: Can't read archive because you didn't provide your archive password."
                    error = True
            elif backup.enc_type in [Backup.RSA, Backup.EL]:
                if len(BackupFile.objects.filter(backup=backup, file_type=BackupFile.PUBLIC_KEY).all()) == 0 or \
                        len(BackupFile.objects.filter(backup=backup, file_type=BackupFile.PRIVATE_KEY).all()) == 0:
                    html += "[<span class=\"red-text\">ERROR</span>]: Can't read archive because you didn't provide your archive keys."
                    error = True
            archive_content = ArchiveContent()
            archive_content.backupfile = backup.get_archives()[0]
            archive_content.status_type = ArchiveContent.WAITING if not error else ArchiveContent.ERROR
            if error:
                archive_content.content = html
            else:
                args = [settings.PACKUP_BIN]
                args.append(os.path.join(settings.MEDIA_ROOT, archive_content.backupfile.backupfile.url))
                lz_dico = backup.get_lz_dico()
                if backup.comp_type == Backup.HUFF:
                    args += ["HUFF", "NONE"]
                elif backup.comp_type == Backup.LZ78 and lz_dico is not None:
                    args += ["LZ", os.path.join(settings.MEDIA_ROOT, lz_dico.backupfile.url)]
                else:
                    args += ["NONE", "NONE"]
                priv_key = backup.get_priv_key()
                if backup.enc_type == Backup.ROTN:
                    args += ["ROTN", backup.pass_backup]
                elif backup.enc_type == Backup.VIG:
                    args += ["VIGENERE", backup.pass_backup]
                elif backup.enc_type == Backup.AES:
                    args += ["AES", backup.pass_backup]
                elif backup.enc_type == Backup.RSA and priv_key is not None:
                    args += ["RSA", os.path.join(settings.MEDIA_ROOT, priv_key.backupfile.url)]
                elif backup.enc_type == Backup.EL and priv_key is not None:
                    args += ["ELGAMAL", os.path.join(settings.MEDIA_ROOT, priv_key.backupfile.url)]
                else:
                    args += ["NONE", "NONE"];
                try:
                    p = subprocess.Popen(args, stdout=subprocess.PIPE, shell=False)
                    (output, err) = p.communicate()
                    p_status = p.wait()
                except:
                    p_status = -1
                if p_status == 0:
                    archive_content.content = "<br/>".join(output.decode("utf-8").split("\n")) 
                    print("\n", archive_content.content, "\n")
                    archive_content.status_type = ArchiveContent.SUCCESS
                else:
                    archive_content.content = "[<span class=\"red-text\">ERROR</span>]: An error occured. Please try again later"
                html += archive_content.content
            archive_content.save()
        else:
            html = archive_content.content
    return HttpResponse(html);
