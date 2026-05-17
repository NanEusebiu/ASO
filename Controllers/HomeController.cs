using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using TodoApp.Data;
using TodoApp.Models;

namespace TodoApp.Controllers;

public class HomeController : Controller
{
    private readonly AppDbContext _db;

    public HomeController(AppDbContext db)
    {
        _db = db;
    }

    // GET: / - Lista tuturor task-urilor
    public async Task<IActionResult> Index(string? filter, string? sort)
    {
        var query = _db.TodoItems.AsQueryable();

        // Filtrare
        query = filter switch
        {
            "active"    => query.Where(t => !t.IsCompleted),
            "completed" => query.Where(t => t.IsCompleted),
            _           => query
        };

        // Sortare
        query = sort switch
        {
            "priority" => query.OrderByDescending(t => t.Priority).ThenBy(t => t.CreatedAt),
            "title"    => query.OrderBy(t => t.Title),
            _          => query.OrderByDescending(t => t.CreatedAt)
        };

        ViewBag.Filter = filter ?? "all";
        ViewBag.Sort   = sort ?? "date";
        ViewBag.Total      = await _db.TodoItems.CountAsync();
        ViewBag.Completed  = await _db.TodoItems.CountAsync(t => t.IsCompleted);

        return View(await query.ToListAsync());
    }

    // POST: /Home/Create - Adaugă un task nou
    [HttpPost]
    [ValidateAntiForgeryToken]
    public async Task<IActionResult> Create(TodoItem item)
    {
        if (!ModelState.IsValid)
            return RedirectToAction(nameof(Index));

        item.CreatedAt   = DateTime.Now;
        item.IsCompleted = false;
        _db.TodoItems.Add(item);
        await _db.SaveChangesAsync();

        TempData["Success"] = $"Task \"{item.Title}\" a fost adaugat.";
        return RedirectToAction(nameof(Index));
    }

    // POST: /Home/Toggle/5 - Comută starea completat/necompletat
    [HttpPost]
    [ValidateAntiForgeryToken]
    public async Task<IActionResult> Toggle(int id)
    {
        var item = await _db.TodoItems.FindAsync(id);
        if (item is null) return NotFound();

        item.IsCompleted = !item.IsCompleted;
        item.CompletedAt = item.IsCompleted ? DateTime.Now : null;
        await _db.SaveChangesAsync();

        return RedirectToAction(nameof(Index));
    }

    // GET: /Home/Edit/5 - Formular de editare
    public async Task<IActionResult> Edit(int id)
    {
        var item = await _db.TodoItems.FindAsync(id);
        if (item is null) return NotFound();
        return View(item);
    }

    // POST: /Home/Edit/5 - Salvează modificările
    [HttpPost]
    [ValidateAntiForgeryToken]
    public async Task<IActionResult> Edit(int id, TodoItem updated)
    {
        if (id != updated.Id) return BadRequest();

        if (!ModelState.IsValid)
            return View(updated);

        var item = await _db.TodoItems.FindAsync(id);
        if (item is null) return NotFound();

        item.Title       = updated.Title;
        item.Description = updated.Description;
        item.Priority    = updated.Priority;

        await _db.SaveChangesAsync();
        TempData["Success"] = "Task actualizat cu succes.";
        return RedirectToAction(nameof(Index));
    }

    // POST: /Home/Delete/5 - Șterge un task
    [HttpPost]
    [ValidateAntiForgeryToken]
    public async Task<IActionResult> Delete(int id)
    {
        var item = await _db.TodoItems.FindAsync(id);
        if (item is null) return NotFound();

        _db.TodoItems.Remove(item);
        await _db.SaveChangesAsync();

        TempData["Success"] = $"Task \"{item.Title}\" a fost sters.";
        return RedirectToAction(nameof(Index));
    }

    // POST: /Home/ClearCompleted - Șterge toate task-urile completate
    [HttpPost]
    [ValidateAntiForgeryToken]
    public async Task<IActionResult> ClearCompleted()
    {
        var completed = await _db.TodoItems.Where(t => t.IsCompleted).ToListAsync();
        _db.TodoItems.RemoveRange(completed);
        await _db.SaveChangesAsync();

        TempData["Success"] = $"{completed.Count} task-uri completate au fost șterse.";
        return RedirectToAction(nameof(Index));
    }
}
