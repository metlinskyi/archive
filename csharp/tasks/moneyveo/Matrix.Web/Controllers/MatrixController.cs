using Matrix.Handlers;
using Matrix.Services;
using Matrix.Web.Models;
using System;
using System.Web;
using System.Web.Mvc;

namespace Matrix.Web.Controllers
{
    public class MatrixController : Controller
    {
        private readonly IMatrixStorage _matrixStorage;

        private readonly IMatrixSerializer _matrixSerializer;

        public MatrixController(IMatrixStorage matrixStorage, IMatrixSerializer matrixSerializer)
        {
            _matrixStorage = matrixStorage;
            _matrixSerializer = matrixSerializer;
        }

        [HttpGet]
        public ActionResult Index(Guid? id)
        {
            if (!id.HasValue)
                return RedirectToAction("Index", "Default");

            var matrix = _matrixStorage.Get(id.Value);

            return View((MatrixModel)matrix);
        }

        [HttpGet]
        public ActionResult Create(uint? size)
        {
            if (!size.HasValue)
                size = (uint)new Random().Next(100, 1000000);

            var matrix = Matrix.Create<int>(size.Value).Handling(new MatrixRandomizer<int>(r => r.Next()));

            Guid id = _matrixStorage.Put(matrix);

            return RedirectToAction("Index", new { id });
        }

        [HttpGet]
        public ActionResult Rotate(Guid id)
        {
            var matrix = _matrixStorage.Get(id).Handling(new MatrixRotation<int>(90));

            return RedirectToAction("Index", new { id });
        }

        [HttpPost]
        public ActionResult Import(HttpPostedFileBase file)
        {
            var matrix = _matrixSerializer.Deserialize(file.InputStream, file.ContentType);

            Guid id = _matrixStorage.Put(matrix);

            return RedirectToAction("Index", new { id });
        }

        [HttpPost]
        public ActionResult Export(Guid id, string contentType)
        {
            var matrix = _matrixStorage.Get(id);

            return File(_matrixSerializer.Serialize(matrix, contentType), contentType);
        }
    }
}